#include <linux/bpf.h>
#include "libbpf.h"
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

