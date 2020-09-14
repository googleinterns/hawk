**This is not an officially supported Google product.**

# hawk

## 1. Overview

HAWK is a userspace tool designed to facilitate the use of KRSI (Kernel Runtime Security Instrumentation). It implements **LSM BPF programs** which are attached to kernel hooks to track different activities in the kernel, using libbpf ringbuffer to exchange data between kernelspace and userspace.


## 2. Usecases

Currently, HAWK supports the following usecases.

### Monitor process executions
- prints information about processes, such as pid, ppid and executable name
- how to use it: ```./hawk --exec```

## 3. Development setup

Start by cloning this repo:
```
git clone https://github.com/Poppy22/hawk.git
```

**Install [gflags](https://github.com/gflags/gflags)**
```
sudo apt-get install libgflags-dev
```

**Linux kernel**

Get the latest version of the Linux kernel:
```
cd  # in your home directory
git clone https://github.com/torvalds/linux.git
```

**libbpf**

Install libbpf with the following commands:
```
cd linux/tools/lib/bpf
make
```

**[bpftool](https://www.mankier.com/8/bpftool)**

This is a tool for inspection and simple manipulation of BPF programs.
```
cd linux/tools/bpf/bpftool
make
sudo make install
```

## 4. Compiling and running HAWK

The compilation process is automated using cmake. After following the set-up instructions, go to `hawk/src` and run the following commands:
```
mkdir build && cd build
cmake ..
make
```
This will generate an executable called `hawk`. Running ```./hawk --help``` will show a list of usage instructions.

To monitor process execution, run:
```
./hawk --exec
```
Then open a new terminal and type commands (`ls` and `cat` for instance). The expected output is:
```
PPID    PID     TGID    PCOM
244     1218    1218    ls
244     1219    1219    ls
244     1220    1220    ls
244     1221    1221    cat
```

## 5. Contributing

If you want to add a new usecase, here's what you should do.

1. Implement the kernelspace and userspace programs for that feature in C:

- create **src/kernelspace/X_monitor.c** (the file that contains the BPF program), which should contain:
	- SEC(str): shows what LSM hook to use to attach to the kernel
	- void BPF_PROG(): the actual code for the BPF program that is executed

- create **src/userspace/X_monitor.c** (the file that contains the userspace program that loads the BPF program into the kernel using libbpf), which should contain:
	- X_monitor(): a primary function that loads the BPF program and polls for data for the ringbuffer
	- a function that shows how the data from the ringbuffer should be consumed (for example, printing to stdout)

- create **src/include/X_monitor.h** (the header file that contains, if needed, the user-defined data structures for that usecase and function headers)

2. Add X to available usecases in the main program (implemented in C++):
**config.cpp**
- include your external C header `X_monitor.h`
- define the command line flags, along with validators, if needed
	- there should be a primary flag that triggers the usecase (for example, `exec`) and optional flags that customize the output
- in `detect_usecase()`, add a check if the primary flag for this usecase is set; don't forget to call the C function `X_monitor()` here, as this has the implementation of the usecase
- if needed, add functions to parse the optional arguments for this usecase

**config.hpp**
- add the necessary variables in the class `Config` for usecase X
- add headers for the primary and helper functions used for usecase X
- add validators headers for flags, if any
