cmake_minimum_required(VERSION 3.29)

# 设置项目名
project(sba_server)

include_directories(include)
link_directories(lib)
aux_source_directory(. a)
aux_source_directory(src a)
add_executable(sba_server ${a})
target_link_libraries(sba_server  ws2_32.lib mysql.lib)