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

#include <linux/bpf.h>
#include "exec.skel.h"
#include <unistd.h>

int main(int ac, char **argv)
{
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

        sleep(20);
        exec__destroy(skel);
        return 0;
}

