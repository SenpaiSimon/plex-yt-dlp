set(LIBNAME ItemIf)

add_library(${LIBNAME} INTERFACE)
target_include_directories(${LIBNAME} INTERFACE ./Include)

add_subdirectory(Source)