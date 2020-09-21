#ifndef MONITOR_H
#define MONITOR_H

class Monitor {
private:
	virtual int export_data() = 0;
public:
	virtual int run() = 0;
};

#endif