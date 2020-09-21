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
#include <bpf/libbpf.h>
#include <unistd.h>
#include <bpf/bpf.h>
#include <iostream>
#include "exec_monitor.skel.h"
#include "process_info.h"
#include "exec_monitor.hpp"

static int process_sample(void *ctx, void *data, size_t len)
{
	if(len < sizeof(struct process_info)) {
		return -1;
	}

	struct process_info *s = (process_info*)data;
	printf("%d\t%d\t%d\t%s\n", s->ppid, s->pid, s->tgid, s->name);
	return 0;
}

ExecMonitor::ExecMonitor(Config config) {
	n_proc = config.n_proc;
	ppid_list = config.ppid_list;
	name_list = config.name_list;
}

int ExecMonitor::run()
{
	struct exec_monitor *skel = NULL;
	skel = exec_monitor__open_and_load();


	if (!skel) {
		std::cerr << "[EXEC_MONITOR]: Error loading the eBPF program.\n";
		exec_monitor__destroy(skel);
		return -1;
	}

	int err = exec_monitor__attach(skel);
	if (err != 0) {
		std::cerr << "[EXEC_MONITOR]: Error attaching the eBPF program.\n";
		exec_monitor__destroy(skel);
		return -1;
	}

	int ringbuffer_fd = bpf_map__fd(skel->maps.ringbuf);
	if (ringbuffer_fd < 0) {
		std::cerr << "[EXEC_MONITOR]: Error accessing the ringbuffer file descriptor.\n";
		exec_monitor__destroy(skel);
		return -1;
	}

	struct ring_buffer *ringbuffer = ring_buffer__new(ringbuffer_fd, process_sample, NULL, NULL);
	if (!ringbuffer) {
		std::cerr << "[EXEC_MONITOR]: Error allocating the ringbufffer.\n";
		exec_monitor__destroy(skel);
		return -1;
	}
	
	printf("PPID\tPID\tTGID\tPCOM\n");
	while (1) {
		// poll for new data with a timeout of -1 ms, waiting indefinitely
		int x = ring_buffer__poll(ringbuffer, -1);
	}

	exec_monitor__destroy(skel);
	return 0;
}
