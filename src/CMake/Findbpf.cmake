FIND_PATH(bpf_INCLUDE_DIR NAMES "bpf/libbpf.h" "bpf/bpf_helpers.h" PATHS ${bpf_INCLUDE_PATH} /include /usr/include /usr/local/include /opt/inc /include/linux/tools /usr/include/linux/tools /opt/inc/linux/tools DOC "The bpf include directory (path to <bpf/libbpf.h>)")
FIND_LIBRARY(bpf_LIBRARIES NAMES "libbpf.a" "bpf" PATHS ${bpf_LIB_PATH} /opt/lib /opt/bpf/lib /usr/lib /usr/local/lib /usr/lib/bpf /usr/local/lib/bpf /lib /lib/bpf /opt/bpf/ ${CMAKE_PREFIX_PATH}/bpf DOC "The path to the bpf library (libbpf.a)")

SET(bpf_FOUND FALSE)

IF(bpf_INCLUDE_DIR AND bpf_LIBRARIES)
  SET(bpf_FOUND TRUE)
  message("Found libbpf. bpf_INCLUDE_DIR: ${bpf_INCLUDE_DIR}; bpf_LIBRARIES: ${bpf_LIBRARIES}")
ELSE()
  MESSAGE(WARNING "Couldn't find bpf libraries. Have you set bpf_INCLUDE_PATH and bpf_LIB_PATH to the root of the bpf lib? Try again with cmake -Dbpf_INCLUDE_DIR=/path/to/bpf/include -Dbpf_LIB_PATH=/path/to/bpf/lib.")
  SET(bpf_FOUND FALSE)
ENDIF()

# Marks the paths as "expert" variables,
# so the gui-user will not modify it (internal only).
MARK_AS_ADVANCED(bpf_INCLUDE_DIR bpf_LIBRARIES)
