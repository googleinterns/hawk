#ifndef CONFIG_HPP
#define CONFIG_HPP

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
	bool valid; /* is set if the command line arguments are valid */

public:
	Config(int argc, char *argv[]);
	Usecase usecase;
	bool is_input_valid();

	// EXEC MONITOR
	std::vector<int> ppid_list;
	std::vector<std::string> name_list;
	int n_proc; /* number of tasks to record */

	// FLAG VALIDATORS
	static bool check_monitor_type(const char *flagname, const std::string &value);
	static bool check_format_type(const char *flagname, const std::string &value);
};

#endif