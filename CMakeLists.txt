cmake_minimum_required(VERSION 3.30.0)
project(yt-dlp-wrapper VERSION 0.1.0 LANGUAGES C CXX)

# lib json
set(JSON_BuildTests OFF CACHE INTERNAL "")
ADD_SUBDIRECTORY(lib/json)
INCLUDE_DIRECTORIES(lib/json/include)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    CUR_VERSION="2.0"
)

# c++ standard
set_property(TARGET ${PROJECT_NAME}
    PROPERTY CXX_STANDARD 23
)

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json 
)

# Compiler Flags
add_compile_options(
    -Wno-deprecated-declarations
    -Wall
    -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
    -Wno-maybe-uninitialized
    -O1
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)