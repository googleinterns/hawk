#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <linux/limits.h>

struct process_info {
	int ppid;
	int pid;
	int tgid;
	char name[PATH_MAX];
};

#endif
