# Basic

## Project
cmake_minimum_required(VERSION 3.16)
project("nebula-atom" LANGUAGES CXX C)
include(cmake/general_options.cmake)
include(cmake/build_settings.cmake)

## Version
include(cmake/version.cmake) 

## Put variables
set(HEADER ${PROJECT_SOURCE_DIR}/src)
list(APPEND HEADER ${PROJECT_BINARY_DIR})
set(PROJECT_FULL_NAME "Nebula Atom")

# Config file
include(GNUInstallDirs)
configure_file(${PROJECT_SOURCE_DIR}/cmake/project_config.h.cmake ${PROJECT_NAME}Config.h)

# Add subdirectories
add_subdirectory(src)

# Add other CMake files
include(cmake/print_informations.cmake)
#include(${PROJECT_SOURCE_DIR}/cmake/cpack.cmake)
