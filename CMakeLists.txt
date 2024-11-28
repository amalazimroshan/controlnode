cmake_minimum_required(VERSION 3.20)
project(controlnode)

set(CMAKE_CXX_STANDARD 20)

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