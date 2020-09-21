#include <iostream>
#include <map>
#include "config.hpp"
#include "exec_monitor.hpp"

int main(int argc, char *argv[])
{
	int ret = 0;
	Config config;

	ret = config.parse_flags(argc, argv);
	if (ret < 0) {
		return ret;
	}

	// the set usecase is certainly valid at this point
	switch (config.usecase) {
	case Usecase::EXEC_MONITOR:
		ExecMonitor exec_monitor(config);
		ret = exec_monitor.run();
		break;
	}

	return ret;
}