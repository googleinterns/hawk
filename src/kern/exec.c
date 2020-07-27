#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

SEC("lsm/bprm_committed_creds")
int BPF_PROG(test_void_hook, struct linux_binprm *bprm)
{
        char str[] = "hello\n";
        bpf_trace_printk(str, sizeof(str));
        return 0;
}

char _license[] SEC("license") = "GPL";

