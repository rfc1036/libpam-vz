/*
 *  Copyright (C) 2000-2008, Parallels, Inc. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef	_TYPES_H_
#define	_TYPES_H_

#define VZ_DIR			PKGCONFDIR "/"
#define GLOBAL_CFG		VZ_DIR "vz.conf"
#define VPS_CONF_DIR		VZ_DIR "conf/"
#define LIB_SCRIPTS_DIR		PKGLIBDIR "/scripts/"
#define DIST_DIR		VZ_DIR "dists"
#define VENAME_DIR		VZ_DIR "names"

#define VZCTLDEV		"/dev/vzctl"

#define VZFIFO_FILE		"/.vzfifo"

#define VPS_STOP		LIB_SCRIPTS_DIR "vps-stop"
#define VPS_NET_ADD		LIB_SCRIPTS_DIR "vps-net_add"
#define VPS_NET_DEL		LIB_SCRIPTS_DIR "vps-net_del"

#define envid_t 	unsigned int
#define STR_SIZE	512
#define PATH_LEN	256

#define NONE		0
#define YES		1
#define NO		2

#define ADD		0
#define DEL		1

/* Default environment variable PATH */
#define	ENV_PATH	"PATH=/bin:/sbin:/usr/bin:/usr/sbin:"

/* CT states */
enum {
	STATE_STARTING = 1,
	STATE_RUNNING = 2,
	STATE_STOPPED = 3,
	STATE_STOPPING = 4,
	STATE_RESTORING = 5,
	STATE_CHECKPOINTING = 6,
};

/* Parse error codes */
#define ERR_DUP		-1
#define ERR_INVAL	-2
#define ERR_UNK		-3
#define ERR_NOMEM	-4
#define ERR_OTHER	-5
#define ERR_INVAL_SKIP	-6
#define ERR_LONG_TRUNC  -7

/** CT handler.
 */
typedef struct {
	int vzfd;	/**< /dev/vzctl file descriptor. */
	int stdfd;
} vps_handler;

typedef enum {
	SKIP_NONE =		0,
	SKIP_SETUP =		(1<<0),
	SKIP_CONFIGURE =	(1<<1),
	SKIP_ACTION_SCRIPT =	(1<<2)
} skipFlags;

typedef int (* execFn)(void *data);

#endif
