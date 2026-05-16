set(LIBNAME Logger) 

add_library(${LIBNAME} STATIC EXCLUDE_FROM_ALL Logger.cpp)

target_include_directories(${LIBNAME} PUBLIC .)
target_link_libraries(${LIBNAME} PUBLIC LoggerIf Thread)