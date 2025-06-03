cmake_minimum_required(VERSION 3.30)
project(skat_list_manager)

set(CMAKE_CXX_STANDARD 20)

add_executable(skat_list_manager main.cpp database_api.cpp terminal_interface.cpp skat.cpp)
target_link_libraries(skat_list_manager mysqlcppconn)
