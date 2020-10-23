SET(Doc "The kernel header directory (path to <linux/bpf.h>)")
IF(KernelHeaders_INCLUDE_PATH)
  FIND_PATH(KernelHeaders_INCLUDE_DIR NAMES "linux/bpf.h" PATHS ${KernelHeaders_INCLUDE_PATH} DOC ${Doc} NO_DEFAULT_PATH)
ELSE()
  FIND_PATH(KernelHeaders_INCLUDE_DIR NAMES "linux/bpf.h" DOC ${Doc})
ENDIF()


SET(KernelHeaders_FOUND FALSE)

IF(KernelHeaders_INCLUDE_DIR)
  SET(KernelHeaders_FOUND TRUE)
  message("Found Linux kernel headers. KernelHeaders_INCLUDE_DIR: ${KernelHeaders_INCLUDE_DIR}")
ELSE()
  MESSAGE(WARNING "Couldn't find kernel headers. Have you set KernelHeaders_INCLUDE_PATH to the directory containing linux/bpf.h? Try again with cmake -DKernelHeaders_INCLUDE_PATH=/path/to/kernel/usr/include/.")
  SET(KernelHeaders_FOUND FALSE)
ENDIF()
