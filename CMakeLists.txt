cmake_minimum_required(VERSION 3.30)
project(skat_list_manager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
file(GLOB SOURCES  src/*.cpp)
add_executable(skat_list_manager ${SOURCES})
target_link_libraries(skat_list_manager PRIVATE Qt6::Widgets mysqlcppconn)
target_sources(skat_list_manager PRIVATE include/main_window.h)
target_sources(skat_list_manager PRIVATE assets/graphics/icons/icons.qrc)
target_include_directories(skat_list_manager PRIVATE include)
