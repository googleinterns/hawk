#ifndef EXEC_MONITOR_H
#define EXEC_MONITOR_H

#include <vector>
#include <string>

class ExecMonitor
{
private:
	std::vector<int> ppid_list;
	std::vector<std::string> name_list;
	int n_proc; /* number of tasks to record */

public:
	ExecMonitor();
	ExecMonitor(std::vector<int> ppid_list, std::vector<std::string> name_list, int n_proc);

	int run();
};

#endif