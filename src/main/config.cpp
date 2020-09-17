#include <cstdio>
#include <iostream>
#include <sstream>
#include "gflags/gflags.h"
#include "config.hpp"
#include "exec_monitor.hpp"

const char DELIMITER = ',';

/*
	FLAGS SECTION
*/

// EXEC FLAG - MONITOR PROCESS EXECUTION
DEFINE_string(ppid, "", "Specifies the parent pid(s) of the processes to monitor using comma-separated values.");
DEFINE_string(name, "", "Specifies the name(s) of the processes to monitor using comma-separated values.");
DEFINE_int32(n, 0, "Specifies the number of processes to monitor. The program will stop after n processes were executed.");
DEFINE_int32(t, 0, "Specifies the number of seconds to monitor. The program will stop after t seconds.");

// DATA EXPORT FLAGS
DEFINE_string(save, "", "Specifies the format in which to save the terminal output in a separate file. Available formats are csv and protobuf.");
DEFINE_validator(save, Config::check_save_and_export_format);
DEFINE_string(export, "", "Specifies the format in which to export data in a separate file. Using export will not print to stdout. Available formats are csv and protobuf.");
DEFINE_validator(export, Config::check_save_and_export_format);

/*
	END OF FLAGS SECTION
*/

Config::Config() {}

int Config::exec_monitor_parse_args_run()
{
	if (!gflags::GetCommandLineFlagInfoOrDie("ppid").is_default)
	{
		std::string token;
		std::istringstream tokenStream(FLAGS_ppid);
		int ppid;
		while (std::getline(tokenStream, token, DELIMITER)) {
			try {
				ppid = stoi(token);
			}
			catch(std::invalid_argument& e) {
				std::cerr << "Unexpected ppid format: " << token << "\n";
				return -1;
			}
			ppid_list.push_back(ppid);
		}
	}

	if (!gflags::GetCommandLineFlagInfoOrDie("name").is_default)
	{
		std::string token;
		std::istringstream tokenStream(FLAGS_name);
		while (std::getline(tokenStream, token, DELIMITER))
			name_list.push_back(token);
	}

	if (!gflags::GetCommandLineFlagInfoOrDie("n").is_default)
		n_proc = (int)FLAGS_n;

	std::cout << "Monitoring process executions...\n";
	ExecMonitor exec_monitor;
	int ret = exec_monitor.run();
	return ret;
}

/*
	FLAG VALIDATORS
*/
bool Config::check_save_and_export_format(const char *flagname, const std::string &value)
{
	// the default value has to be in the validator, otherwise error
	return (value.compare("csv") == 0 or value.compare("protobuf") == 0 or value.compare("") == 0);
}