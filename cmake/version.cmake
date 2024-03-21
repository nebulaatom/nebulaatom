
# Read current version
file(READ ${CMAKE_SOURCE_DIR}/version.txt CURRENT_VERSION)
set(VERSION ${CURRENT_VERSION})

# Get version from Git
execute_process(
    COMMAND git describe --tags --abbrev=0
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE NEW_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Verify new Git version
if(NOT NEW_VERSION STREQUAL "")
    if(NOT NEW_VERSION STREQUAL CURRENT_VERSION)
        set(VERSION ${NEW_VERSION})
        file(WRITE ${CMAKE_SOURCE_DIR}/version.txt ${NEW_VERSION})
    endif()
endif()

# Setting up variables
set(PACKAGE_VERSION "${VERSION}")
set(PACKAGE_VERSION_COMPLETE "${VERSION}")
message(STATUS "Package version: ${PACKAGE_VERSION}")
