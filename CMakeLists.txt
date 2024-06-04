# Basic

## Project
cmake_minimum_required(VERSION 3.16)
project("nebulaatom" LANGUAGES CXX C)
set(PROJECT_FULL_NAME "Nebula Atom")

## Include CMake files
include(cmake/general_options.cmake)
include(cmake/build_settings.cmake)
include(cmake/find_packages.cmake)

# Add subdirectories
include(GNUInstallDirs)
add_subdirectory(src)

# Add other CMake files
include(cmake/version.cmake) 
include(cmake/print_informations.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/cpack.cmake)

# Config file
configure_file(${PROJECT_SOURCE_DIR}/cmake/project_config.h.cmake ${PROJECT_NAME}Config.h)
