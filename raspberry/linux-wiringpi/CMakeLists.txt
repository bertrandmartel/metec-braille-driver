cmake_minimum_required(VERSION 3.0)
project(metecdriver LANGUAGES CXX VERSION 1.0)
set(example_dir "out/examples")
set (CMAKE_CXX_STANDARD 11)
include(GNUInstallDirs)
add_library(metecdriver SHARED ../../src/braille.cpp)
set_target_properties(metecdriver PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    LIBRARY_OUTPUT_DIRECTORY out
    PUBLIC_HEADER ../../include/braille.h)
target_include_directories(metecdriver PRIVATE ../../include)
set (WPI_PATH ./raspberry-dev/wiringPi/wiringPi)
include_directories (include ${WPI_PATH})

#library install
install(TARGETS metecdriver
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#link wiringpi
find_library(WPI_LIB wiringPi HINTS ${WPI_PATH} NO_CMAKE_FIND_ROOT_PATH)
if(NOT WPI_LIB)
message(FATAL_ERROR "wiringPi library not found")
endif()

#link liblouis
set (LIBLOUIS_PATH ./liblouis/liblouis/.libs)
find_library(LOUIS_LIB louis HINTS ${LIBLOUIS_PATH} NO_CMAKE_FIND_ROOT_PATH)
if(NOT LOUIS_LIB)
message(FATAL_ERROR "liblouis library not found")
endif()

#iterate examples
file( GLOB EXAMPLES_SRC examples/*.cpp )
foreach( testsourcefile ${EXAMPLES_SRC} )
	get_filename_component(base_name ${testsourcefile} NAME_WE)
    add_executable(${base_name} ${testsourcefile})
    message(${base_name})
    set_target_properties(${base_name} 
    	PROPERTIES 
    		OUTPUT_NAME ${base_name}
    		RUNTIME_OUTPUT_DIRECTORY ${example_dir})
    target_link_libraries(${base_name} ${WPI_LIB})
    target_link_libraries(${base_name} ${LOUIS_LIB})
    target_link_libraries( ${base_name} metecdriver )
    target_include_directories(${base_name} PUBLIC ../../include)
    target_include_directories(${base_name} PUBLIC ./liblouis/liblouis)
endforeach( testsourcefile ${EXAMPLES_SRC} )
