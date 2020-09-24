FIND_PATH(KernelHeaders_INCLUDE_DIR NAMES "linux/bpf.h" PATHS ${KernelHeaders_INCLUDE_PATH} /include /usr/include DOC "The kernel header directory (path to <linux/bpf.h>)")

SET(KernelHeaders_FOUND FALSE)

IF(KernelHeaders_INCLUDE_DIR)
  SET(KernelHeaders_FOUND TRUE)
  message("Found Linux kernel headers. KernelHeaders_INCLUDE_DIR: ${bpf_INCLUDE_DIR}")
MESSAGE(WARNING "Couldn't find kernel headers. Have you set KernelHeaders_INCLUDE_PATH to the directory containing linux/bpf.h? Try again with cmake -DKernelHeaders_INCLUDE_DIR=/path/to/kernel/include/uapi/.")
  SET(KernelHeaders_FOUND FALSE)
ENDIF()
