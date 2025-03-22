cmake_minimum_required(VERSION 3.0)
project(server)

# 编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# header files search directories
include_directories(/usr/include)
link_directories(/usr/lib)

add_executable(testserver ./testmuduo.cpp)

target_link_libraries(testserver muduo_net muduo_base pthread)

