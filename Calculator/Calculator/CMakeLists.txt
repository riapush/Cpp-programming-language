cmake_minimum_required(VERSION 3.23)
project(Calculator)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_HEADERS PluginLoader.h Operations.h Calc.h)
add_executable(Calculator ${PROJECT_SOURCE_DIR}/ main.cpp Operation.cpp PluginLoader.cpp Calc.cpp)
add_library(sin MODULE ./plugins/sin.cpp)