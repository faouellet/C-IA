cmake_minimum_required (VERSION 3.0)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS unit_test_framework)

find_package(Boost COMPONENTS unit_test_framework)
if(Boost_FOUND)
    if(WIN32)
		file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
        
        set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
		
        # Boost
        include_directories("${Boost_INCLUDE_DIR}")
        link_directories("${Boost_LIBRARY_DIRS}")
        
        add_executable(Tests ${SOURCES}) 
        target_link_libraries(Tests ${Boost_LIBRARIES} distlanglib)
    else()
        include(../cmake/BoostTestHelpers.cmake)

		# TosLang tests
		add_boost_test(matrix.cpp distlanglib)
    endif()
endif()