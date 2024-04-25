project (sunnet)
cmake_minimum_required(VERSION 3.22)
include_directories(net_base/include lockstep/include math/include)
aux_source_directory(./net_base/src DIR_SRCS)
aux_source_directory(./lockstep/src DIR_LS)
aux_source_directory(./math/src DIR_TS)

# 格式： 可执行文件名字 main文件位置 其他源码（cpp）位置
add_executable(sunnet ./net_base/main.cpp ${DIR_SRCS})
target_link_libraries(sunnet liblua.a)
target_link_libraries(sunnet dl)

add_executable(lockstep ./lockstep/main.cpp ${DIR_LS})

add_executable(math ./math/main.cpp ${DIR_TS})

add_executable(totalMain ./main.cpp ${DIR_SRCS} ${DIR_LS} ${DIR_TS})
# 记得链接库
target_link_libraries(totalMain liblua.a)
target_link_libraries(totalMain dl)
