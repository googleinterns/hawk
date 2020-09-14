#ifndef EXEC_MONITOR_H
#define EXEC_MONITOR_H

#include <linux/limits.h>

struct process_info {
	int ppid;
	int pid;
	int tgid;
	char name[PATH_MAX];
};

void exec_monitor();

#endif
