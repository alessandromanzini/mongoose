# "cmake-lists": swiftlog, "author": alessandromanzini
# top-level cmake file, includes the lib and test subdirectories.
#
cmake_minimum_required( VERSION 3.26 )
project( swiftlog
         VERSION 0.1.0
         DESCRIPTION "A modern C++ logging library"
         LANGUAGES CXX )


# ========================================
# GLOBAL CONFIGURATION
# ========================================

# c++ standard configuration
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
message( STATUS "C++${CMAKE_CXX_STANDARD} support has been enabled by default." )

# build type configuration
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE )
    set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo" )
endif()
message( STATUS "Build Type: ${CMAKE_BUILD_TYPE}" )


# ========================================
# COMPILER-SPECIFIC SETTINGS
# ========================================

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
    message( STATUS "Using Clang compiler configuration" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    message( STATUS "Using GCC compiler configuration" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
    message( STATUS "Using MSVC compiler configuration" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" ) # enable parallel compilation on MSVC
endif()


# ========================================
# OUTPUT DIRECTORIES
# ========================================

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

# set output directories for all build configurations
foreach( OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/lib )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/lib )
endforeach()


# ========================================
# CMAKE MODULE PATH
# ========================================

# append cmake module path, making sure cmake finds our custom modules (.cmake)
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )


# ========================================
# PROJECT OPTIONS
# ========================================

option( SWIFTLOG_BUILD_TESTS "Build test executable" ON )
#option( SWIFTLOG_BUILD_EXAMPLES "Build example applications" OFF )
#option( SWIFTLOG_BUILD_DOCUMENTATION "Build documentation" OFF )
#option( SWIFTLOG_ENABLE_PROFILING "Enable profiling support" OFF )


# ========================================
# ADD SUBDIRECTORIES
# ========================================

# core library
add_subdirectory( "lib" )

# test executable
if( SWIFTLOG_BUILD_TESTS )
    add_subdirectory( "tests" )
endif()
