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
#include "exec.skel.h"
#include <unistd.h>
#include <bpf/bpf.h>

int main(int ac, char **argv)
{
	int cpus_nb = libbpf_num_possible_cpus();
	unsigned long long executions_nb_on_cpu[cpus_nb];
	int map_fd;
	int total_executions = 0;
	unsigned long long key = 0;

	struct exec *skel = NULL;
	skel = exec__open_and_load();

	if (!skel) {
		printf("Error loading the program.\n");
		exec__destroy(skel);
		return 0;
	}

	int err = exec__attach(skel);
	if (err != 0) {
		printf("Error attaching the program.\n");
		exec__destroy(skel);
		return 0;
	}

	// get the fd for the map created in the BPF program
	map_fd = bpf_map__fd(skel->maps.output_map);

	// count the total number of processes executed on the CPUs each second
	while (1) {
		/*
		 * Search for the element with key = 0 in the map.
		 * values will point to output_map[key]
		 */
		bpf_map_lookup_elem(map_fd, &key, executions_nb_on_cpu);

		// reset the counter to avoid counting duplicates
		total_executions = 0;

		// accumulate execution number for all CPUs
		for (int i = 0; i < cpus_nb; i++) {
			total_executions += executions_nb_on_cpu[i];
		}

		printf("There were %d processes executed on %d CPUs\n", total_executions, cpus_nb);
		sleep(1);
	}

	exec__destroy(skel);
	return 0;
}
