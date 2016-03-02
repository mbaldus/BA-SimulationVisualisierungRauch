#
# Try to find OpenCL library and include path.
# Once done this will define
#
# OPENCL_FOUND
# OPENCL_INCLUDE_PATH
# OPENCL_LIBRARY
# 

SET(OPENCL_SEARCH_PATHS
	$ENV{CUDA_PATH_V7_5}
	${DEPENDENCIES_ROOT})


FIND_PATH(OPENCL_INCLUDE_PATH
    NAMES
        CL/cl.h
    PATHS
        ${OPENCL_SEARCH_PATHS}
    PATH_SUFFIXES
        include
    DOC
        "The directory where CL/cl.h resides"
)

FIND_LIBRARY(OPENCL_LIBRARY
    NAMES
        OpenCL.lib
    PATHS
        ${OPENGL_SEARCH_PATHS}
    PATH_SUFFIXES
        lib
        Win32
        /lib/Win32
    DOC
        "The directory where OpenCL.lib resides"
)

SET(OPENCL_FOUND "NO")
IF (OPENCL_INCLUDE_PATH AND OPENCL_LIBRARY)
    SET(OPENCL_LIBRARIES ${OPENCL_LIBRARY})
    SET(OPENCL_FOUND "YES")
    message("EXTERNAL LIBRARY 'OPENCL' FOUND")
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'OPENCL' NOT FOUND")
ENDIF (OPENCL_INCLUDE_PATH AND OPENCL_LIBRARY)
