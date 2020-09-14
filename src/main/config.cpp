#include <iostream>
#include <sstream>
#include "gflags/gflags.h"
#include "config.hpp"

extern "C" {
	#include "exec_monitor.h"
}

/*
	FLAGS SECTION
*/

// EXEC FLAG - MONITOR PROCESS EXECUTION
DEFINE_bool(exec, false, "Monitors process executions.");
DEFINE_string(ppid, "", "Specifies the ppid(s) of the processes to monitor using comma-separated values.");
DEFINE_string(name, "", "Specifies the name(s) of the processes to monitor using comma-separated values.");
DEFINE_int32(n, 0, "Specifies the number of processes to monitor. The program will stop after n processes were executed.");
DEFINE_int32(t, 0, "Specifies the number of seconds to monitor. The program will stop after t seconds.");

// DATA EXPORT FLAGS
DEFINE_string(save, "", "Specifies the format in which to save the terminal output in a separate file. Available formats are csv and protobuf.");
DEFINE_validator(save, Config::checkSaveAndExportFormat);
DEFINE_string(export, "", "Specifies the format in which to export data in a separate file. Using export will not print to stdout. Available formats are csv and protobuf.");
DEFINE_validator(export, Config::checkSaveAndExportFormat);

// END OF FLAGS SECTION

Config::Config()
{
}

void Config::detect_usecase(int argc, char *argv[])
{
	gflags::SetUsageMessage("");
	gflags::SetVersionString("1.0.0");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	if (FLAGS_exec)
	{
		exec_monitor_parse_args_run();
		std::cout << "Monitoring process executions...\n";
		std::cout << "PPID\tPID\tTGID\tPCOM\n";
		
		// Call the BPF program
		exec_monitor();
		return;
	}

	std::cout << "Incorrect use. Run hawk --help for instructions.\n";
}

void Config::exec_monitor_parse_args_run()
{
	if (!gflags::GetCommandLineFlagInfoOrDie("ppid").is_default)
	{
		std::string token;
		std::istringstream tokenStream(FLAGS_ppid);
		while (std::getline(tokenStream, token, DELIMITER))
			ppid.push_back(stoi(token));
	}

	if (!gflags::GetCommandLineFlagInfoOrDie("name").is_default)
	{
		std::string token;
		std::istringstream tokenStream(FLAGS_name);
		while (std::getline(tokenStream, token, DELIMITER))
			name.push_back(token);
	}

	if (!gflags::GetCommandLineFlagInfoOrDie("n").is_default)
		n_proc = (int)FLAGS_n;

	gflags::ShutDownCommandLineFlags();
}

/*
	FLAG VALIDATORS
*/
bool Config::checkSaveAndExportFormat(const char *flagname, const std::string &value)
{
	// the default value has to be in the validator, otherwise error
	return (value.compare("csv") == 0 or value.compare("protobuf") == 0 or value.compare("") == 0);
}