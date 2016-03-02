project(KERNELS)

file(GLOB_RECURSE CL *.cl)

SOURCE_GROUP(Kernel FILES ${CL})

add_custom_target(KERNELS SOURCES ${CL})