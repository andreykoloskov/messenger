cmake_minimum_required(VERSION 2.8)
project(messenger)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARIES boost_system boost_thread-mt)

set(PROJECTS_INCLUDE_DIR include)
set (CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/core/include)

set(PROJECTS_SOURCE_DIR src)
set (CORE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/core/src)

add_definitions(-Wall -std=c++11)

add_subdirectory(client)
add_subdirectory(server)
