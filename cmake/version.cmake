# Get Git tag
execute_process(
    COMMAND git describe --tags --abbrev=0
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set variables
set(PACKAGE_VERSION "${VERSION}")
set(PACKAGE_VERSION_COMPLETE "${VERSION}")

message(STATUS "Package version: ${PACKAGE_VERSION}")