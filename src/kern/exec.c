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

/*
 * Define a map structure to communicate with the userspace program.
 * The type of map is BPF_MAP_TYPE_PERCPU_ARRAY;
 * it contains one element, found for key = 0
 * and the value is an array with number_of_CPUs elements.
 */
struct bpf_map_def SEC("maps") output_map = {
        .type = BPF_MAP_TYPE_PERCPU_ARRAY,
        .key_size = sizeof(int),
        .value_size = sizeof(int),
        .max_entries = 1,
};

SEC("lsm/bprm_committed_creds")
int BPF_PROG(test_void_hook, struct linux_binprm *bprm)
{
        char str[] = "hello\n";
        int key = 0; // the only key in the map, since there is just one key-value pair

        // print a string for each executed process
        bpf_trace_printk(str, sizeof(str));

        /*
         * Obtain the current value found at key = 0 and on the current CPU.
         * val points to output_map[key][i], where i is the index of the current
         * CPU; this index is already known by the BPF program, as it internally
         * keeps track of the CPUs.
         */
        int *val = bpf_map_lookup_elem(&output_map, &key);
        if (!val) {
                return 0;
        }

        // Increment the previous value, as a new process was executed on the
        // current CPU.
        (*val)++;
        return 0;
}

char _license[] SEC("license") = "GPL";

