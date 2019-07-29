cmake_minimum_required (VERSION 3.0)
project (DistLang)

enable_testing()

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core orcjit)

add_subdirectory (src)
add_subdirectory (tests)

add_executable (DistLang main.cpp)
set_target_properties(distlanglib PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(DistLang distlanglib ${llvm_libs}) 