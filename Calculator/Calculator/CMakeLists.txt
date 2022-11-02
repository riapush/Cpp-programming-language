cmake_minimum_required(VERSION 3.23)
project(Calculator)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_HEADERS PluginLoader.h Operations.h Calc.h)
add_executable(Calculator ${PROJECT_SOURCE_DIR}/ main.cpp Operation.cpp PluginLoader.cpp Calc.cpp)
add_library(sin MODULE ./plugins/sin.cpp)
add_library(cos MODULE ./plugins/cos.cpp)
add_library(tg MODULE ./plugins/tg.cpp)
add_library(ln MODULE ./plugins/ln.cpp)
add_library(sqrt MODULE ./plugins/sqrt.cpp)