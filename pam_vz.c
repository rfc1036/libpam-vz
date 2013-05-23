/*
 * Copyright 2009 by Marco d'Itri <md@linux.it>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>

#define PAM_SM_SESSION
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <security/pam_modutil.h>

struct pam_vz_config {
    int min_id;
    int max_id;
    int use_gid;
    int ctid_delta;
    int closelog;
};

/* Prototypes */
extern int ve_enter(const id_t ctid);

static int doit(pam_handle_t *pamh, struct pam_vz_config *config);
static gid_t get_user_group_id(pam_handle_t *pamh, const int use_gid);
static int parse_option(pam_handle_t *pamh, struct pam_vz_config *config,
	const char *argv);
static int parse_options(pam_handle_t *pamh, struct pam_vz_config *config,
	const int argc, const char **argv);

/****************************************************************************/
/* Configuration defaults */
static struct pam_vz_config default_config = {
    .min_id = 10000,	/* totally arbitrary */
    .max_id = 65533,	/* (nobody - 1) */
    .use_gid = 0,	/* by default use the UID as CTID */
    .ctid_delta = 0,
    .closelog = 0,	/* close the syslog socket after entering the CT */
};

/* global flag used by log_d() */
static int debug = 0;

/* Convenience macros */
#define streq(a, b) (strcmp(a, b) == 0)
#define strneq(a, b, n) (strncmp(a, b, n) == 0)
#define strneq2(a, b) (strncmp(a, b, strlen(b)) == 0)

#define log_d(x, ...) do { \
    if (debug) \
	 pam_syslog(pamh, LOG_DEBUG,  x, ## __VA_ARGS__); \
} while(0);

#define log_n(x, ...) \
    do { pam_syslog(pamh, LOG_NOTICE, x, ## __VA_ARGS__); } while(0);

#define log_e(x, ...) \
    do { pam_syslog(pamh, LOG_ERR,    x, ## __VA_ARGS__); } while(0);

/****************************************************************************/
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc,
			    const char **argv)
{
    struct pam_vz_config *config = &default_config;
    int rc;

    log_d("pam_sm_open_session called");

    rc = parse_options(pamh, config, argc, argv);
    if (rc != PAM_SUCCESS)
	return rc;

    if (!doit(pamh, config))
	return PAM_ABORT;

    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc,
			     const char **argv)
{
    log_d("pam_sm_close_session called");

    return PAM_IGNORE;
}

/****************************************************************************/
static int doit(pam_handle_t *pamh, struct pam_vz_config *config) {
    int rc;
    id_t ctid;

    /* Get the GID corresponding to the user opening the session.
     * -1 is used to report an error condition in the hope that nobody
     * would want to associate it to a CTID...
     */
    ctid = get_user_group_id(pamh, config->use_gid);
    if (ctid == -1)
	return 0;

    /* if the ID is not in the range then do nothing and return success */
    if (ctid < config->min_id || ctid > config->max_id) {
	log_d("%s %u is not in the CTIDs range, not entering a container",
		config->use_gid ? "gid" : "uid", ctid);
	return 1;
    }

    ctid += config->ctid_delta;

    log_n("trying to enter container %u", ctid);

    rc = ve_enter(ctid);

    /* Beware: in normal conditions the precedent call to syslog will cause
     * /dev/log in the HN to be opened and continue to be used even after
     * entering the container.
     * If the closelog option is specified then it will be closed and
     * after this point the process will try to log in the container (and
     * fail if it does not have a running syslog daemon).
     */
    if (rc == 1 && config->closelog)
	closelog();

    if (rc == 1) {
	log_n("entered container %u", ctid);
    } else
	log_e("failed entering container %u", ctid);

    return rc;
}

/****************************************************************************/
static gid_t get_user_group_id(pam_handle_t *pamh, const int use_gid)
{
    const char *user;
    struct passwd const *pw;
    int ret;

    ret = pam_get_item(pamh, PAM_USER, (const void **) &user);

    if (ret != PAM_SUCCESS) {
	log_e("pam_get_item(PAM_USER): %s", pam_strerror(pamh, ret));
	return -1;
    }

    errno = 0;
    pw = pam_modutil_getpwnam(pamh, user);
    if (!pw) {
	log_e("getpwnam(%s): %m", user);
	return -1;
    }

    return use_gid ? pw->pw_gid : pw->pw_uid;
}

/****************************************************************************/
static int parse_option(pam_handle_t *pamh, struct pam_vz_config *config,
	const char *argv)
{
    if        (streq(argv, "debug")) {
	debug = 1;
    } else if (strneq2(argv, "min_id=")) {
	errno = 0;
	config->min_id = strtol(argv + 8, NULL, 10);
	if (errno == ERANGE || errno == EINVAL || config->min_id == 0) {
	    log_e("unparseable value: %s", argv);
	    return PAM_SESSION_ERR;
	}
    } else if (strneq2(argv, "max_id=")) {
	errno = 0;
	config->max_id = strtol(argv + 7, NULL, 10);
	if (errno == ERANGE || errno == EINVAL || config->max_id == 0) {
	    log_e("unparseable value: %s", argv);
	    return PAM_SESSION_ERR;
	}
    } else if (strneq2(argv, "use_as_ctid=")) {
	if        (streq(argv + 12, "uid")) {
	    config->use_gid = 0;
	} else if (streq(argv + 12, "gid")) {
	    config->use_gid = 1;
	} else {
	    log_e("invalid argument: %s", argv);
	    return PAM_SESSION_ERR;
	}
    } else if (strneq2(argv, "ctid_delta=")) {
	errno = 0;
	config->ctid_delta = strtol(argv + 7, NULL, 10);
	if (errno == ERANGE || errno == EINVAL) {
	    log_e("unparseable value: %s", argv);
	    return PAM_SESSION_ERR;
	}
    } else if (streq(argv, "closelog")) {
	config->closelog = 1;
    } else {
	log_e("invalid config option: %s", argv);
	return PAM_SESSION_ERR;
    }
    return PAM_SUCCESS;
}

/* parse the module configuration directives */
static int parse_options(pam_handle_t *pamh, struct pam_vz_config *config,
	const int argc, const char **argv)
{
    int i;

    for (i = 0; i < argc; i++) {
	if (argv == NULL || argv[0] == '\0')
	    continue;

	int rc = parse_option(pamh, config, argv[i]);
	if (rc != PAM_SUCCESS)
	    return rc;
    }

    return PAM_SUCCESS;
}

/****************************************************************************/
#ifdef PAM_STATIC
/* used when the module is built static */
struct pam_module _pam_vz_modstruct = {
    "pam_vz",
    NULL,
    NULL,
    NULL,
    pam_sm_open_session,
    pam_sm_close_session,
    NULL,
};
#endif

