cmake_minimum_required(VERSION 3.5.0)
project(plex-yt-dlp VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# lib curl
ADD_SUBDIRECTORY(lib/curlcpp)
INCLUDE_DIRECTORIES(lib/curlcpp/include)
include_directories(${CURL_INCLUDE_DIR})

# lib json
set(JSON_BuildTests OFF CACHE INTERNAL "")
ADD_SUBDIRECTORY(lib/json)
INCLUDE_DIRECTORIES(lib/json/include)

INCLUDE_DIRECTORIES(include)

add_executable(${PROJECT_NAME}
    src/misc/colors.cpp
    src/misc/tools.cpp
    src/argumentParser/settings.cpp
    src/argumentParser/argumentParser.cpp
    src/config.cpp
    src/idExtractor.cpp
    src/downloader/downloader.cpp
    src/main.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    CUR_VERSION="1.2"
)

# c++ standard
set_property(TARGET ${PROJECT_NAME}
    PROPERTY CXX_STANDARD 17
)

if(MSVC)
    add_compile_definitions(_WIN32)
    add_compile_definitions(CURL_STATICLIB)
endif()

if(WIN32)
    add_compile_definitions(_WIN32)
    add_compile_definitions(CURL_STATICLIB)
else()
    add_compile_definitions(__linux__)
endif()

# Libraries
target_link_libraries(${PROJECT_NAME}
    ${CURL_LIBRARY}
    curlcpp
    nlohmann_json::nlohmann_json 
)

# Compiler Flags
add_compile_options(
    -Wno-deprecated-declarations
    -Wall
    -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
    -Wno-maybe-uninitialized
    -O3
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)