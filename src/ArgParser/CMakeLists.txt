set(LIBNAME ArgParser)

add_library(${LIBNAME} STATIC ArgParser.cpp)

target_include_directories(${LIBNAME} PUBLIC .)

target_link_libraries(${LIBNAME} 
  PUBLIC
    LoggerIf
)