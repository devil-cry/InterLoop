cmake_minimum_required(VERSION 3.20)
project(InterLoop)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#add_definitions(-D_DEBUG)

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/share DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/bedpe DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/logger DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/io DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/view DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/solver DIR_SRCS)

add_executable(InterLoop InterLoop.cpp ${DIR_SRCS})
