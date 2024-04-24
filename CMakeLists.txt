project (sunnet)
cmake_minimum_required(VERSION 3.22)
include_directories(include)
aux_source_directory(./src DIR_SRCS)
add_executable(sunnet ${DIR_SRCS})
target_link_libraries(sunnet ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(sunnet liblua.a)
target_link_libraries(sunnet dl)