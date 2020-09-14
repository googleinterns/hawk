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
#include "exec_monitor.skel.h"
#include <unistd.h>
#include <bpf/bpf.h>
#include "exec_monitor.h"

static int process_sample(void *ctx, void *data, size_t len)
{
	struct process_info *s = data;
	printf("%d\t%d\t%d\t%s\n", s->ppid, s->pid, s->tgid, s->name);
	return 0;
}

void exec_monitor()
{
	int ringbuffer_fd;
	struct ring_buffer *ringbuffer;

	struct exec_monitor *skel = NULL;
	skel = exec_monitor__open_and_load();

	if (!skel) {
		printf("Error loading the program.\n");
		exec_monitor__destroy(skel);
		return;
	}

	int err = exec_monitor__attach(skel);
	if (err != 0) {
		printf("Error attaching the program.\n");
		exec_monitor__destroy(skel);
		return;
	}

	ringbuffer_fd = bpf_map__fd(skel->maps.ringbuf);
	if (ringbuffer_fd < 0) {
		printf("Error accessing the ringbuffer.\n");
		exec_monitor__destroy(skel);
		return;
	}

	ringbuffer = ring_buffer__new(ringbuffer_fd, process_sample, NULL, NULL);
	if (!ringbuffer) {
		printf("Error creating the ringbufffer.\n");
		exec_monitor__destroy(skel);
		return;
	}
	
	while (1) {
		// poll for new data with a timeout of -1 ms, waiting indefinitely
		ring_buffer__poll(ringbuffer, -1);
	}

	exec_monitor__destroy(skel);
}
