/*
 * SPDX-FileCopyrightText: Stone Tickle <lattis@mochiro.moe>
 * SPDX-FileCopyrightText: Vincent Torri <vtorri@outlook.fr>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#ifdef MUON_HAVE_GETLOADAVG
#define _BSD_SOURCE
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#endif

#include <unistd.h>

#include "log.h"
#include "platform/os.h"

bool os_chdir(const char *path)
{
	return chdir(path) == 0;
}

char *os_getcwd(char *buf, size_t size)
{
	return getcwd(buf, size);
}

int os_getopt(int argc, char * const argv[], const char *optstring)
{
	return getopt(argc, argv, optstring);
}

uint32_t
os_parallel_job_count(void)
{
#ifdef _SC_NPROCESSORS_ONLN
	int n = sysconf(_SC_NPROCESSORS_ONLN);
	if (n == -1) {
		return 4;
	} else if (n < 2) {
		return 2;
	} else {
		return n + 2;
	}
#else
	return 4;
#endif
}

double
os_getloadavg(void)
{
#ifdef MUON_HAVE_GETLOADAVG
	double load;

	if (getloadavg(&load, 1) == -1) {
		LOG_W("failed: getloadavg: %s", strerror(errno));
		load = 100.0;
	}

	return load;
#else
	return 0;
#endif
}
