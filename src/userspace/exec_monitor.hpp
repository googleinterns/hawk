#ifndef EXEC_MONITOR_H
#define EXEC_MONITOR_H

#include <vector>
#include <string>
#include "../main/config.hpp"

class ExecMonitor
{
private:
	std::vector<int> ppid_list;
	std::vector<std::string> name_list;
	int n_proc; /* number of tasks to record */

public:
	ExecMonitor(Config config);
	int run();
};

#endif