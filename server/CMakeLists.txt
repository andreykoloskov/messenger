cmake_minimum_required(VERSION 2.8)
project(server)

include_directories(${PROJECTS_INCLUDE_DIR})
include_directories(${CORE_INCLUDE_DIR})

aux_source_directory(${PROJECTS_SOURCE_DIR} SRC)
aux_source_directory(${CORE_SOURCE_DIR} CORE_SRC)

add_executable(${PROJECT_NAME} ${SRC} ${CORE_SRC})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
