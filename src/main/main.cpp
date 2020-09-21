#include <iostream>
#include <map>
#include <gflags/gflags.h>
#include "config.hpp"

DEFINE_string(monitor, "exec", "Defines the usecase of the program. The default value is \"exec\", which monitors process executions.");

std::map<std::string, Usecase> usecase_map = {
    {"exec", EXEC_MONITOR}
};

bool existing_usecase(std::string key, std::map<std::string, Usecase> usecase_map)
{
	return usecase_map.count(key);
}

int main(int argc, char *argv[])
{
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	int ret = 0;

	if (!existing_usecase(FLAGS_monitor, usecase_map)) {
		std::cerr << "Incorrect value for monitor. Run hawk --help for instructions.\n";
		gflags::ShutDownCommandLineFlags();
		return -1;
	}

	Config config;
	switch (usecase_map[FLAGS_monitor]) {
	case Usecase::EXEC_MONITOR:
		ret = config.exec_monitor_parse_args_run();
		break;
	default:
		std::cerr << "Incorrect use. Run hawk --help for instructions.\n";
	}

	gflags::ShutDownCommandLineFlags();
	return ret;
}