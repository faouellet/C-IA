cmake_minimum_required (VERSION 2.8)

add_executable (C++IA main.cpp)
llvm_map_components_to_libnames(llvm_libs support core)
target_link_libraries(C++IA ${llvm_libs}) 
