set(LIBNAME Item)

add_library(${LIBNAME} STATIC Playlist.cpp Single.cpp)

target_include_directories(${LIBNAME} PUBLIC .)

target_link_libraries(${LIBNAME} 
  PUBLIC
    ItemIf
    LoggerIf
)

