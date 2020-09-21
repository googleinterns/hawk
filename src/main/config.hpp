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
private:
	int exec_monitor_parse_args();

public:
	Config();
	Usecase usecase;
	int parse_flags(int argc, char *argv[]);

	// EXEC MONITOR
	std::vector<int> ppid_list;
	std::vector<std::string> name_list;
	int n_proc; /* number of tasks to record */

	// FLAG VALIDATORS
	static bool check_monitor_type(const char *flagname, const std::string &value);
	static bool check_format_type(const char *flagname, const std::string &value);
};

#endif