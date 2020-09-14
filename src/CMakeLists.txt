cmake_minimum_required(VERSION 3.13)

project(Hawk)

add_custom_command(
        COMMENT "Building exec_monitor_user.o"
        OUTPUT exec_monitor_user.o
        COMMAND bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
        COMMAND clang -g -D__TARGET_ARCH_x86 -mlittle-endian -Wno-compare-distinct-pointer-types -I../include -I../build -O2 -target bpf -emit-llvm -c ../kernelspace/exec_monitor.c -o - | llc -march=bpf -mcpu=v2 -filetype=obj -o exec_monitor.o
        COMMAND bpftool gen skeleton exec_monitor.o > exec_monitor.skel.h
        COMMAND gcc -g ../userspace/exec_monitor.c -c -o exec_monitor_user.o -I. -I../include
        DEPENDS ./kernelspace/exec_monitor.c ./userspace/exec_monitor.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(gflags REQUIRED)

set(SOURCE_FILES
        "main/config.cpp"
        "main/main.cpp"
)

#include_directories(/root/linux/tools/lib/bpf/)
add_executable(hawk ${SOURCE_FILES} exec_monitor_user.o)
target_include_directories(hawk PUBLIC ${gflags_INCLUDE_DIR} include)
target_link_libraries(hawk gflags /root/linux/tools/lib/bpf/libbpf.a elf z)