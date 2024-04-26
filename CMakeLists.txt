project (sunnet)
cmake_minimum_required(VERSION 3.22)
include_directories(net_base/include lockstep/include math/include math_3d/include)
aux_source_directory(./net_base/src DIR_NET)
aux_source_directory(./lockstep/src DIR_LS)
aux_source_directory(./math/src DIR_MATH)
aux_source_directory(./math_3d/src DIR_MATH3D)

# 格式： 可执行文件名字 main文件位置 其他源码（cpp）位置
add_executable(sunnet ./net_base/main.cpp ${DIR_NET})
target_link_libraries(sunnet liblua.a)
target_link_libraries(sunnet dl)

add_executable(lockstep ./lockstep/main.cpp ${DIR_LS})

add_executable(math ./math/main.cpp ${DIR_MATH})

# 3D引用了Math
add_executable(3dmath ./math_3d/main.cpp ${DIR_MATH} ${DIR_MATH3D})

add_executable(totalMain ./main.cpp ${DIR_NET} ${DIR_LS} ${DIR_MATH})
# 记得链接库
target_link_libraries(totalMain liblua.a)
target_link_libraries(totalMain dl)
