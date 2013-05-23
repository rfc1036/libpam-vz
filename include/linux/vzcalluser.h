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

#ifndef _LINUX_VZCALLUSER_H
#define _LINUX_VZCALLUSER_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define KERN_VZ_PRIV_RANGE 51

#ifndef __ENVID_T_DEFINED__
typedef unsigned envid_t;
#define __ENVID_T_DEFINED__
#endif

#ifndef __KERNEL__
#define __user
#endif

/*
 * CT management ioctls
 */

struct vzctl_old_env_create {
	envid_t veid;
	unsigned flags;
#define VE_CREATE 	1	/* Create CT, VE_ENTER added automatically */
#define VE_EXCLUSIVE	2	/* Fail if exists */
#define VE_ENTER	4	/* Enter existing CT */
#define VE_TEST		8	/* Test if CT exists */
#define VE_LOCK		16	/* Do not allow entering created CT */
#define VE_SKIPLOCK	32	/* Allow entering "embryo" CT */
	__u32 addr;
};

struct vzctl_mark_env_to_down {
	envid_t veid;
};

struct vzctl_setdevperms {
	envid_t veid;
	unsigned type;
#define VE_USE_MAJOR	010	/* Test MAJOR supplied in rule */
#define VE_USE_MINOR	030	/* Test MINOR supplied in rule */
#define VE_USE_MASK	030	/* Testing mask, VE_USE_MAJOR|VE_USE_MINOR */
	unsigned dev;
	unsigned mask;
};

struct vzctl_ve_netdev {
	envid_t veid;
	int op;
#define VE_NETDEV_ADD  1
#define VE_NETDEV_DEL  2
	char __user *dev_name;
};

struct vzctl_ve_meminfo {
	envid_t veid;
	unsigned long val;
};

/* these masks represent modules */
#define VE_IP_IPTABLES_MOD		(1U<<0)
#define VE_IP_FILTER_MOD		(1U<<1)
#define VE_IP_MANGLE_MOD		(1U<<2)
#define VE_IP_MATCH_LIMIT_MOD		(1U<<3)
#define VE_IP_MATCH_MULTIPORT_MOD	(1U<<4)
#define VE_IP_MATCH_TOS_MOD		(1U<<5)
#define VE_IP_TARGET_TOS_MOD		(1U<<6)
#define VE_IP_TARGET_REJECT_MOD		(1U<<7)
#define VE_IP_TARGET_TCPMSS_MOD		(1U<<8)
#define VE_IP_MATCH_TCPMSS_MOD		(1U<<9)
#define VE_IP_MATCH_TTL_MOD		(1U<<10)
#define VE_IP_TARGET_LOG_MOD		(1U<<11)
#define VE_IP_MATCH_LENGTH_MOD		(1U<<12)
#define VE_IP_CONNTRACK_MOD		(1U<<14)
#define VE_IP_CONNTRACK_FTP_MOD		(1U<<15)
#define VE_IP_CONNTRACK_IRC_MOD		(1U<<16)
#define VE_IP_MATCH_CONNTRACK_MOD	(1U<<17)
#define VE_IP_MATCH_STATE_MOD		(1U<<18)
#define VE_IP_MATCH_HELPER_MOD		(1U<<19)
#define VE_IP_NAT_MOD			(1U<<20)
#define VE_IP_NAT_FTP_MOD		(1U<<21)
#define VE_IP_NAT_IRC_MOD		(1U<<22)
#define VE_IP_TARGET_REDIRECT_MOD	(1U<<23)
#define VE_IP_MATCH_OWNER_MOD		(1U<<24)
#define VE_IP_MATCH_MAC_MOD		(1U<<25)

/* these masks represent modules with their dependences */
#define VE_IP_IPTABLES		(VE_IP_IPTABLES_MOD)
#define VE_IP_FILTER		(VE_IP_FILTER_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_MANGLE		(VE_IP_MANGLE_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_MATCH_LIMIT	(VE_IP_MATCH_LIMIT_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_MATCH_MULTIPORT	(VE_IP_MATCH_MULTIPORT_MOD	\
					| VE_IP_IPTABLES)
#define VE_IP_MATCH_TOS		(VE_IP_MATCH_TOS_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_TARGET_TOS	(VE_IP_TARGET_TOS_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_TARGET_REJECT	(VE_IP_TARGET_REJECT_MOD	\
					| VE_IP_IPTABLES)
#define VE_IP_TARGET_TCPMSS	(VE_IP_TARGET_TCPMSS_MOD	\
					| VE_IP_IPTABLES)
#define VE_IP_MATCH_TCPMSS	(VE_IP_MATCH_TCPMSS_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_MATCH_TTL		(VE_IP_MATCH_TTL_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_TARGET_LOG	(VE_IP_TARGET_LOG_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_MATCH_LENGTH	(VE_IP_MATCH_LENGTH_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_CONNTRACK		(VE_IP_CONNTRACK_MOD		\
					| VE_IP_IPTABLES)
#define VE_IP_CONNTRACK_FTP	(VE_IP_CONNTRACK_FTP_MOD	\
					| VE_IP_CONNTRACK)
#define VE_IP_CONNTRACK_IRC	(VE_IP_CONNTRACK_IRC_MOD	\
					| VE_IP_CONNTRACK)
#define VE_IP_MATCH_CONNTRACK	(VE_IP_MATCH_CONNTRACK_MOD	\
					| VE_IP_CONNTRACK)
#define VE_IP_MATCH_STATE	(VE_IP_MATCH_STATE_MOD		\
					| VE_IP_CONNTRACK)
#define VE_IP_MATCH_HELPER	(VE_IP_MATCH_HELPER_MOD		\
					| VE_IP_CONNTRACK)
#define VE_IP_NAT		(VE_IP_NAT_MOD			\
					| VE_IP_CONNTRACK)
#define VE_IP_NAT_FTP		(VE_IP_NAT_FTP_MOD		\
					| VE_IP_NAT | VE_IP_CONNTRACK_FTP)
#define VE_IP_NAT_IRC		(VE_IP_NAT_IRC_MOD		\
					| VE_IP_NAT | VE_IP_CONNTRACK_IRC)
#define VE_IP_TARGET_REDIRECT	(VE_IP_TARGET_REDIRECT_MOD	\
					| VE_IP_NAT)
#define VE_IP_MATCH_MAC		(VE_IP_MATCH_MAC_MOD	\
					| VE_IP_IPTABLES)
#define VE_IP_MATCH_OWNER	(VE_IP_MATCH_OWNER_MOD	\
					| VE_IP_IPTABLES)

/* safe iptables mask to be used by default */
#define VE_IP_DEFAULT					\
	(VE_IP_IPTABLES |				\
	VE_IP_FILTER | VE_IP_MANGLE |			\
	VE_IP_MATCH_LIMIT | VE_IP_MATCH_MULTIPORT |	\
	VE_IP_MATCH_TOS | VE_IP_TARGET_REJECT | 	\
	VE_IP_TARGET_TCPMSS | VE_IP_MATCH_TCPMSS |	\
	VE_IP_MATCH_TTL | VE_IP_MATCH_LENGTH)

#define VE_IPT_CMP(x,y)		(((x) & (y)) == (y))

struct vzctl_env_create_cid {
	envid_t veid;
	unsigned flags;
	__u32 class_id;
};

struct vzctl_env_create {
	envid_t veid;
	unsigned flags;
	__u32 class_id;
};

struct env_create_param {
	__u64 iptables_mask;
};

#define VZCTL_ENV_CREATE_DATA_MINLEN	sizeof(struct env_create_param)

struct env_create_param2 {
	__u64 iptables_mask;
	__u64 feature_mask;
	__u32 total_vcpus;	/* 0 - don't care, same as in host */
};

struct env_create_param3 {
	__u64 iptables_mask;
	__u64 feature_mask;
	__u32 total_vcpus;
	__u32 pad;
	__u64 known_features;
};

#define VE_FEATURE_SYSFS	(1ULL << 0)
#define VE_FEATURE_NFS		(1ULL << 1)
#define VE_FEATURE_DEF_PERMS	(1ULL << 2)
#define VE_FEATURE_SIT		(1ULL << 3)
#define VE_FEATURE_IPIP		(1ULL << 4)

#define VE_FEATURES_OLD		(VE_FEATURE_SYSFS)
#define VE_FEATURES_DEF		(VE_FEATURE_SYSFS | \
				 VE_FEATURE_DEF_PERMS)

typedef struct env_create_param3 env_create_param_t;
#define VZCTL_ENV_CREATE_DATA_MAXLEN	sizeof(env_create_param_t)

struct vzctl_env_create_data {
	envid_t veid;
	unsigned flags;
	__u32 class_id;
	env_create_param_t __user *data;
	int datalen;
};

struct vz_load_avg {
	int val_int;
	int val_frac;
};

struct vz_cpu_stat {
	unsigned long user_jif;
	unsigned long nice_jif;
	unsigned long system_jif;
	unsigned long uptime_jif;
	__u64 idle_clk;
	__u64 strv_clk;
	__u64 uptime_clk;
	struct vz_load_avg avenrun[3];	/* loadavg data */
};

struct vzctl_cpustatctl {
	envid_t veid;
	struct vz_cpu_stat __user *cpustat;
};

#define VZCTLTYPE '.'
#define VZCTL_OLD_ENV_CREATE	_IOW(VZCTLTYPE, 0,			\
					struct vzctl_old_env_create)
#define VZCTL_MARK_ENV_TO_DOWN	_IOW(VZCTLTYPE, 1,			\
					struct vzctl_mark_env_to_down)
#define VZCTL_SETDEVPERMS	_IOW(VZCTLTYPE, 2,			\
					struct vzctl_setdevperms)
#define VZCTL_ENV_CREATE_CID	_IOW(VZCTLTYPE, 4,			\
					struct vzctl_env_create_cid)
#define VZCTL_ENV_CREATE	_IOW(VZCTLTYPE, 5,			\
					struct vzctl_env_create)
#define VZCTL_GET_CPU_STAT	_IOW(VZCTLTYPE, 6,			\
					struct vzctl_cpustatctl)
#define VZCTL_ENV_CREATE_DATA	_IOW(VZCTLTYPE, 10,			\
					struct vzctl_env_create_data)
#define VZCTL_VE_NETDEV		_IOW(VZCTLTYPE, 11,			\
					struct vzctl_ve_netdev)
#define VZCTL_VE_MEMINFO	_IOW(VZCTLTYPE, 13,                     \
					struct vzctl_ve_meminfo)

#ifdef __KERNEL__
#include <linux/compat.h>
#ifdef CONFIG_COMPAT
struct compat_vzctl_ve_netdev {
	envid_t veid;
	int op;
	compat_uptr_t dev_name;
};

struct compat_vzctl_ve_meminfo {
	envid_t veid;
	compat_ulong_t val;
};

struct compat_vzctl_env_create_data {
	envid_t veid;
	unsigned flags;
	__u32 class_id;
	compat_uptr_t data;
	int datalen;
};

#define VZCTL_COMPAT_ENV_CREATE_DATA _IOW(VZCTLTYPE, 10,		\
					struct compat_vzctl_env_create_data)
#define VZCTL_COMPAT_VE_NETDEV	_IOW(VZCTLTYPE, 11,			\
					struct compat_vzctl_ve_netdev)
#define VZCTL_COMPAT_VE_MEMINFO	_IOW(VZCTLTYPE, 13,                     \
					struct compat_vzctl_ve_meminfo)
#endif
#endif

#endif
