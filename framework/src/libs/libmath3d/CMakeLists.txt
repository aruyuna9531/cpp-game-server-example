cmake_minimum_required(VERSION 2.8)

if (CMAKE_BUILD_TYPE STREQUAL Release) 
	set(MyProjectName math3d)
else()
	set(CMAKE_BUILD_TYPE Debug)
	set(MyProjectName math3dd)
endif()

message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "ProjectName:${MyProjectName}")

project(${MyProjectName})

aux_source_directory(. SRCS)
aux_source_directory(../proto SRCS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../../libs")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_library(${MyProjectName} STATIC ${SRCS})