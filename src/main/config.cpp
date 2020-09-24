#include <cstdio>
#include <iostream>
#include <sstream>
#include <gflags/gflags.h>
#include "config.hpp"

const char DELIMITER = ',';

/*
	FLAGS SECTION
*/

DEFINE_string(monitor, "exec", "Defines the usecase of the program. The default value is \"exec\", which monitors process executions.");
DEFINE_validator(monitor, Config::check_monitor_type);

// EXEC FLAG - MONITOR PROCESS EXECUTION
DEFINE_string(ppid, "", "Specifies the parent pid(s) of the processes to monitor using comma-separated values.");
DEFINE_string(name, "", "Specifies the name(s) of the processes to monitor using comma-separated values.");
DEFINE_int32(n, 0, "Specifies the number of processes to monitor. The program will stop after n processes were executed.");

// DATA EXPORT FLAGS
DEFINE_string(format, "", "Specifies the format in which to save the terminal output in a separate file. Available formats are csv and protobuf.");
DEFINE_validator(format, Config::check_format_type);
DEFINE_string(output_file, "", "Specifies the path to write the ouput or where to create the file, if it does not exist at that path.");

/*
	END OF FLAGS SECTION
*/

std::map<std::string, Usecase> usecase_map = {
    {"exec", EXEC_MONITOR}
};

Config::Config(int argc, char *argv[])
{
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	valid = true;

	switch (usecase_map[FLAGS_monitor]) {
	case Usecase::EXEC_MONITOR:
		usecase = EXEC_MONITOR;
		if (exec_monitor_parse_args() < 0)
			valid = false;
		break;
	}

	gflags::ShutDownCommandLineFlags();
}

bool Config::is_input_valid() {
	return valid;
}

int Config::exec_monitor_parse_args()
{
	if (!gflags::GetCommandLineFlagInfoOrDie("ppid").is_default)
	{
		std::string token;
		std::istringstream tokenStream(FLAGS_ppid);
		int ppid;
		while (std::getline(tokenStream, token, DELIMITER))
		{
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

	return 0;
}

/*
	FLAG VALIDATORS
*/
bool Config::check_monitor_type(const char *flagname, const std::string &value)
{
	return usecase_map.count(value);
}

bool Config::check_format_type(const char *flagname, const std::string &value)
{
	// the default value has to be in the validator, otherwise error
	return (value.compare("csv") == 0 or value.compare("protobuf") == 0 or value.compare("") == 0);
}