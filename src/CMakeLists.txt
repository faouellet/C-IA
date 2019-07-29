cmake_minimum_required (VERSION 3.0)

file(GLOB DIST_SOURCES "*.cpp" "*.h")

add_library( distlanglib STATIC
		${DIST_SOURCES}
		)
				
target_link_libraries(distlanglib ${llvm_libs})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
