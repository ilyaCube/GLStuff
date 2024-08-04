cmake_minimum_required(VERSION 3.22)

project("noName" LANGUAGES CXX)

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "1")
set(PROJECT_VERSION_PATCH "1")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS *)

list(APPEND LINKED_LIBRARIES Lib)

# Imports
include("Qt")

add_executable(${PROJECT_NAME} ${SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINKED_LIBRARIES})