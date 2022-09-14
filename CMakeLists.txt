cmake_minimum_required(VERSION 3.4)

project(Channels)

set(CMAKE_CXX_STANDARD 20)

include(cmake/Ccache.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Development.cmake)
include(cmake/Helpers.cmake)
include(cmake/Platform.cmake)
include(cmake/Sanitize.cmake)

add_subdirectory(3rdparty)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
				message(STATUS "Enable tests")
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

add_executable(channel main.cpp Queue.hpp Channel.hpp Select.hpp)

target_link_libraries(channel libcoro)

