#ifndef EXEC_MONITOR_HPP
#define EXEC_MONITOR_HPP

#include <vector>
#include <string>
#include "../main/config.hpp"
#include "monitor.hpp"

class ExecMonitor : public Monitor
{
private:
	std::vector<int> ppid_list;
	std::vector<std::string> name_list;
	int n_proc; /* number of tasks to record */

	int export_data();

public:
	ExecMonitor(Config config);
	int run();
};

#endif