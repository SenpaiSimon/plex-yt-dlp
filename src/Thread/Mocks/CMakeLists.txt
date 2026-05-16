set(LIBNAME ThreadMock)

add_library(${LIBNAME} STATIC EXCLUDE_FROM_ALL Thread.cpp)

target_include_directories(${LIBNAME} 
PUBLIC
  .
)

target_link_libraries(
${LIBNAME} 
PUBLIC
  pthread
  ThreadIf
)