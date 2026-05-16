set(LIBNAME LoggerIf)

add_library(${LIBNAME} INTERFACE)
target_include_directories(${LIBNAME} INTERFACE ./Include)


# add_subdirectory(Test)
add_subdirectory(Source)