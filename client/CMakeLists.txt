cmake_minimum_required(VERSION 2.8)
project(client)

include_directories(${PROJECTS_INCLUDE_DIR})
aux_source_directory(${PROJECTS_SOURCE_DIR} SRC)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
