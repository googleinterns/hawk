/*
 * Copyright 2020 Google LLC
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include <linux/limits.h>

struct record_sample {
	int ppid;
	int pid;
	int tgid;
	char name[PATH_MAX];
};

struct {
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__uint(max_entries, 1 << 24);
} ringbuf SEC(".maps");

long ringbuf_flags = 0;

SEC("lsm/bprm_committed_creds")
int BPF_PROG(test_void_hook, struct linux_binprm *bprm)
{
	int pid, tgid, ppid;
	struct record_sample *sample;
	struct task_struct *current_task;

	// Get information about the current proces
	pid = bpf_get_current_pid_tgid() >> 32;
	tgid = (bpf_get_current_pid_tgid() << 32) >> 32;
	ppid = -1;

	sample = bpf_ringbuf_reserve(&ringbuf, sizeof(*sample), ringbuf_flags);
	if (!sample)
		return 1;

	// Get the parent pid
	current_task = (struct task_struct *)bpf_get_current_task();
	ppid = BPF_CORE_READ(current_task, real_parent, pid);

	// Get the executable name
	bpf_get_current_comm(&sample->name, sizeof(sample->name));

	sample->ppid = ppid;
	sample->pid = pid;
	sample->tgid = tgid;

	bpf_ringbuf_submit(sample, ringbuf_flags);
	return 0;
}

char _license[] SEC("license") = "GPL";

