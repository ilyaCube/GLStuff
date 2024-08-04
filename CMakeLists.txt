cmake_minimum_required(VERSION 3.22)

project("noName" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Cmake")

add_subdirectory("Lib")
add_subdirectory("App")