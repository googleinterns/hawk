#include <iostream>
#include "config.hpp"
#include "exec_monitor.hpp"

int main(int argc, char *argv[])
{
	int ret = 0;
	Config config(argc, argv);

	if (!config.is_input_valid())
		return ret;

	// the set usecase is certainly valid at this point
	switch (config.usecase) {
	case Usecase::EXEC_MONITOR:
		ExecMonitor exec_monitor(config);
		ret = exec_monitor.run();
		break;
	}

	return ret;
}