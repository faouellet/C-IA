cmake_minimum_required (VERSION 3.0)

file(GLOB DIST_SOURCES "*.cpp" "*.h")

add_library( distlanglib STATIC
		${DIST_SOURCES}
		)
				
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
