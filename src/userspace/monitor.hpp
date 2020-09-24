#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>
class Monitor {
private:
	/*
		Function called if the --format flag is specified.
		Prints the execution summary in the specified format.
	*/
	virtual int export_data() = 0;

public:
	/*
		This is the actual implementation of the usecase.
		Should load and attach the eBPF object, poll for data and process each sample.
	*/
	virtual int run() = 0;
};

#endif