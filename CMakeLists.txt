cmake_minimum_required(VERSION 3.20)
project(InterLoop)

set(CMAKE_CXX_STANDARD 20)
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
# cmd example
# -f ../res/dataset_6_valid.bedpe -o example -t 3 -ws 10000 -lv DEBUG -lo my.log -inc_pair chr2-chr12,chr1-chr2

aux_source_directory(${PROJECT_SOURCE_DIR}/src/benchmark DIR_SRCS)
add_executable(test_acc ${PROJECT_SOURCE_DIR}/src/benchmark/test_acc.cpp ${DIR_SRCS})
# -gf ../res/example_golden.bedpe -tf ../res/example_test.bedpe -d 100000
