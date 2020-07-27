#!/bin/bash

echo "Open a new terminal and use the following command: sudo cat /sys/kernel/debug/tracing/trace"

kcc()
{
  clang -g -D__TARGET_ARCH_x86 -mlittle-endian -Wno-compare-distinct-pointer-types -O2 -target bpf -emit-llvm -c $1 -o - | llc -march=bpf -mcpu=v2 -filetype=obj -o "$(basename  $1 .c).o";
}

ucc ()
{
    gcc -g $1 -o "$(basename $1 .c)" -I$HOME/libbpf/src/ $HOME/libbpf/src/libbpf.a -lelf -lz
}

rm -f ../src/user/user
rm -f ../src/kern/exec.o

bpftool btf dump file /sys/kernel/btf/vmlinux format c > ../src/kern/vmlinux.h
kcc ../src/kern/exec.c
mv  exec.o ../src/kern/exec.o

bpftool gen skeleton ../src/kern/exec.o > ../src/user/exec.skel.h
ucc ../src/user/user.c
mv user ../src/user/user

sudo ../src/user/user
