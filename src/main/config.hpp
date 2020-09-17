#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <map>

enum Usecase {
	EXEC_MONITOR /* monitors process execution */
};

class Config
{
	// EXEC MONITOR
	std::vector<int> ppid_list;
	std::vector<std::string> name_list;
	int n_proc; /* number of tasks to record */

public:
	Config();
	static std::map<std::string, Usecase> usecase_map;

	int exec_monitor_parse_args_run();

	// VALIDATORS
	static bool check_save_and_export_format(const char *flagname, const std::string &value);
};

#endif