cmake_minimum_required (VERSION 2.8)
project (DistLang)

enable_testing()

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory (src)
add_subdirectory (tests)