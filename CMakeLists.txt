cmake_minimum_required(VERSION 3.14)
project(callpass)

set(CMAKE_CXX_STANDARD 17)
find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS}")
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
message(${CMAKE_CXX_FLAGS})

include_directories(.)
add_library(callpass MODULE main.cpp )
set_property(TARGET "callpass" APPEND_STRING PROPERTY LINK_FLAGS "${LLVM_LINK_FLAGS}")
