cmake_minimum_required(VERSION 3.13)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

project(Hawk)

# TODO: improve cmake
list(APPEND files "exec_monitor")

foreach(filename ${files})
	add_custom_command(
		COMMENT "Building"
		OUTPUT ${filename}_user.o
		COMMAND bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
		COMMAND clang -g -D__TARGET_ARCH_x86 -mlittle-endian -Wno-compare-distinct-pointer-types -I../include -I../build -I${bpf_INCLUDE_DIR} -O2 -target bpf -emit-llvm -c ../kernelspace/${filename}.c -o - | llc -march=bpf -mcpu=v2 -filetype=obj -o ${filename}.o
                COMMAND bpftool gen skeleton ${filename}.o > ${filename}.skel.h
		COMMAND g++ -g ../userspace/${filename}.cpp -c -o ${filename}_user.o -I. -I../include -I${bpf_INCLUDE_DIR}
		DEPENDS ./kernelspace/${filename}.c ./userspace/${filename}.cpp
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
endforeach()

find_package(gflags REQUIRED)
find_package(bpf REQUIRED)

set(SOURCE_FILES
        "main/config.cpp"
        "main/main.cpp"
)
set(BPFToolPath "/root/linux/tools/lib/bpf")
list(TRANSFORM files APPEND "_user.o")

add_executable(hawk ${SOURCE_FILES} ${files})
target_include_directories(hawk PUBLIC ${gflags_INCLUDE_DIR} ${bpf_INCLUDE_DIR} include userspace)
target_link_libraries(hawk ${gflags_LIBRARIES} ${bpf_LIBRARIES} ${BPFToolPath}/libbpf.a elf z)
