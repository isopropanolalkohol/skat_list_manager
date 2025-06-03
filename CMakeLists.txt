cmake_minimum_required(VERSION 3.30)
project(skat_list_manager)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp")

add_executable(skat_list_manager ${SOURCES})
target_link_libraries(skat_list_manager mysqlcppconn)


target_include_directories(skat_list_manager PRIVATE include)
