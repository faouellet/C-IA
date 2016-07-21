cmake_minimum_required (VERSION 3.0)
project (DistLang)

enable_testing()

add_subdirectory (src)
add_subdirectory (tests)

add_executable (DistLang main.cpp)
SET_TARGET_PROPERTIES(distlanglib PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(DistLang distlanglib) 