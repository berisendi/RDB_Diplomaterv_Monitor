# The minimum required version is dictated by the usage of Qt5
cmake_minimum_required(VERSION 3.1.0)

# Defining the projectname
project(RDB_Diplomaterv_Monitor)

# Project version: Major_Minor_Patch
# Rules:
#	The version numbers are only to be increased by 1
#	When increasing a version number, the lower level version numbers need to be reseted to 0
#	Major: Increase it with architectural changes
#	Minor: Increase it with functional changes that include interface changes as well
#	Patch: Increase it with changes that do not include interface changes 
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}_${PROJECT_VERSION_PATCH}")

# Looking for the external libraries
message(STATUS "Looking for the external libraries...")

# Looking for Qt
# Setting up the environment variables to be able to find the required libraries
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.11.1/gcc_64/lib/cmake)
message(STATUS "The value of CMAKE_PREFIX_PATH is: ${CMAKE_PREFIX_PATH}")
find_package(Qt5 COMPONENTS Core Widgets Charts REQUIRED)
# Instruct CMake to run Qt moc preprocessor automatically when needed
set(CMAKE_AUTOMOC ON)

# Looking for Boost
find_package(Boost COMPONENTS system REQUIRED)

# Setting up the compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -std=c++14 -pthread")

# Adding the subdirectories
add_subdirectory(sources)
