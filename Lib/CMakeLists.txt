cmake_minimum_required(VERSION 3.22)

project("Lib" LANGUAGES CXX)

include("Qt")
include("OpenCV")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS *)

add_library(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ".")
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBRARIES})