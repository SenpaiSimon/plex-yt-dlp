cmake_minimum_required(VERSION 3.30.0)
project(yt-dlp-wrapper VERSION 0.1.0 LANGUAGES C CXX)

# get googletest
# cmake_policy(SET CMP0135 NEW)
# include(FetchContent)

# FetchContent_Declare(
#     googletest
#     URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
#     SYSTEM
# )

# # Forcing GTest to use the same runtime as your project (important for Windows)
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# FetchContent_MakeAvailable(googletest)

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

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Thread
    Logger
    Item
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