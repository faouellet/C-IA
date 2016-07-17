cmake_minimum_required (VERSION 2.8)

add_executable (DistLang main.cpp)
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(DistLang ${llvm_libs}) 
