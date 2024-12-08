cmake_minimum_required(VERSION 3.20)
project(controlnode)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

find_package(CURL QUIET)
if(NOT CURL_FOUND)
    message(STATUS "CURL not found. Downloading and building it using FetchContent")
    FetchContent_Declare(
        CURL
        GIT_REPOSITORY https://github.com/curl/curl.git
        GIT_TAG        curl-8_3_0 
    )
    FetchContent_MakeAvailable(CURL)
endif()

find_package(CURL REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(controlnode
    ${HEADERS}
    ${SOURCES}
)

target_include_directories(controlnode PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CURL_INCLUDE_DIRS}"
)

target_link_libraries(controlnode PUBLIC CURL::libcurl)