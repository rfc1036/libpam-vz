/*
 * Copyright 2009 by Marco d'Itri <md@linux.it>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE

/* System library */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>

/* OpenVZ-related includes */
#include "linux/vzcalluser.h"
#include "vzsyscalls.h"
#include "types.h"

/* FIXME: this is the Debian default, other systems may use /vz/ */
#undef VZ_DIR
#define VZ_DIR "/var/lib/vz/"

/* Prototypes */
int ve_enter(const id_t veid);
static inline int setluid(const id_t uid);

#define log_e(x, ...) do { \
    syslog(LOG_AUTHPRIV|LOG_ERR, "pam_vz: " x, ## __VA_ARGS__); \
} while(0);

int ve_enter(const id_t veid)
{
    int vzfd;
    int errcode;
    int retry = 0;
    struct vzctl_env_create env_create;
    char *root;

    if ((vzfd = open(VZCTLDEV, O_RDWR)) < 0) {
	log_e("open(%s): %m", VZCTLDEV);
	return 0;
    }

    if (asprintf(&root, "%sroot/%u/", VZ_DIR, veid) == -1) {
	log_e("malloc: %m");
	close(vzfd);
	return 0;
    }

    if (chroot(root) < 0) {
	log_e("chroot(%s): %m", root);
	free(root);
	close(vzfd);
	return 0;
    }

    free(root);

    if (chdir("/") < 0) {
	log_e("chdir(/): %m");
	close(vzfd);
	return 0;
    }

    /* associate the process with its beancounter */
    if (setluid(veid) < 0) {
	log_e("setluid(%d): %m", veid);
	close(vzfd);
	return 0;
    }

    memset(&env_create, 0, sizeof(env_create));
    env_create.veid = veid;
    env_create.flags = VE_ENTER;

    /* enter the VE */
    do {
	if (retry)
	    sleep(1);
	errcode = ioctl(vzfd, VZCTL_ENV_CREATE, &env_create);
    } while (errcode < 0 && errno == EBUSY && retry++ < 3);

    close(vzfd);

    if (errcode < 0) {
	log_e("ioctl(VZCTL_ENV_CREATE): %m");
	return 0;
    }

    return 1;
}

/****************************************************************************/
static inline int setluid(const id_t uid)
{
    return syscall(__NR_setluid, uid);
}

/****************************************************************************/
#ifdef TEST

int main(int argc, char *argv[], char *env[])
{
    id_t veid = 10000;

    if (!*++argv) {
	fprintf(stderr, "Usage: ve_enter PROGRAM [ARG]...\n");
	exit(1);
    }

    if (!ve_enter(veid)) {
	fprintf(stderr, "ve_enter failed!\n");
	exit(1);
    }

    execv(argv[0], argv);

    fprintf(stderr, "exec(%s): %s\n", argv[0], strerror(errno));
    exit(1);
}

#endif

