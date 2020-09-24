**This is not an officially supported Google product.**

# hawk

## 1. Overview

HAWK is a userspace tool designed to facilitate the use of KRSI (Kernel Runtime Security Instrumentation). It implements **LSM BPF programs** which are attached to kernel hooks to track different activities in the kernel, using libbpf ringbuffer to exchange data between kernelspace and userspace.


## 2. Usecases

HAWK has a mandatory flag that should be used whenever run, `--monitor`. The value specified shows the desired usecase, i.e. what should be monitored. Currently, HAWK supports the following usecase:

### Monitor process executions
- prints information about processes, such as pid, ppid and executable name
- how to use it: ```./hawk --monitor=exec```
- optional flags:
	- --ppid: comma-separated list; monitors only processes that have their ppid in the given list
	- --name: comma-separated list; monitors only processed that have their executable name in the given list
	- -n: number; monitors the first n processes that are executed after running hawk

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

Build libbpf:
```
make -C linux/tools/lib/bpf
```

**If you're running Hawk against a custom kernel**

(If you're running it on the same machine where you build it, you don't need
this bit. You just need the kernel tree to build libbpf and bpftool). Obviously
you'll also have to build the kernel, and install it wherever you want to run
Hawk - that's out of the scope of this document!

```
KERNEL_TREE=$PWD/linux # Remember this for later.
make headers_install   # Generate UAPI headers to compile Hawk against.
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
```

- To build for running on your local machine:

  ```
  cmake .. && make
  ```

- To build for running on your custom kernel:

  ```
  cmake -Dbpf_INCLUDE_DIR=$KERNEL_TREE/tools/lib/ -Dbpf_LIB_PATH=$KERNEL_TREE/tools/lib/bpf -Dlinux_INCLUDE_DIR=$KERNEL_TREE/usr/include/..
  make
  ```

This will generate an executable called `hawk`. Running ```./hawk --help``` will show a list of usage instructions.

To monitor process execution, run:
```
./hawk --monitor=exec
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

If you want to add a new usecase X, here's what you should do.

1. Implement the kernelspace program for that usecase in C:

- create **src/kernelspace/X_monitor.c** (the file that contains the BPF program), which should contain:
	- SEC(str): shows what LSM hook to use to attach to the kernel
	- void BPF_PROG(): the actual code for the BPF program that is executed

2. Implement the userspace class for that usecase in C++ in **src/userspace**. This class contains the userspace program that loads the BPF program into the kernel using libbpf. It should contain:
- a primary function that loads the BPF program and polls for data for the ringbuffer
- a function that shows how the data from the ringbuffer should be consumed (for example, printing to stdout)

Should you need additional user defined structures for this usecase, create headers in **src/include**.

3. Add X to available usecases in the main program:

**config.hpp**
- add the usecase to the enum `Usecase`
- add validators headers for flags, if any

**config.cpp**
- define the command line flags, along with validators, if needed
- define a function to parse the flags for this usecase

**main.cpp**
- add a case for this particular usecase in the `switch` instruction
