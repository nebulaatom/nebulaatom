# Basic

## Project
cmake_minimum_required(VERSION 3.16)
project("cpw-woodpecker-server" LANGUAGES CXX C)
include(cmake/general_options.cmake)
include(cmake/build_settings.cmake)
include(cmake/FindMySQL.cmake)

## Version
set(${PROJECT_NAME}_VERSION_MAYOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 8)
set(${PROJECT_NAME}_VERSION_PATCH 12)
set(${PROJECT_NAME}_VERSION_SUFFIX "-alfa")

## Put variables
set(HEADER ${PROJECT_SOURCE_DIR}/src)
list(APPEND HEADER ${PROJECT_BINARY_DIR})
set(PROJECT_FULL_NAME "CPW Woodpecker Server")
set(
    PACKAGE_VERSION
    ${${PROJECT_NAME}_VERSION_MAYOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}
)
set(
    PACKAGE_VERSION_COMPLETE
    ${PACKAGE_VERSION}${${PROJECT_NAME}_VERSION_SUFFIX}
)

# Config file
include(GNUInstallDirs)
configure_file(${PROJECT_SOURCE_DIR}/cmake/project_config.h.cmake ${PROJECT_NAME}Config.h)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(share)

# Add other CMake files

include(cmake/files_install.cmake)
include(cmake/print_informations.cmake)
