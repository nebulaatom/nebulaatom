# Testing
if(${PROJECT_NAME}_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()

# Find packages
include(${PROJECT_SOURCE_DIR}/cmake/FindMySQL.cmake)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util Data DataMySQL)
if(NOT Poco_FOUND)
	message(FATAL_ERROR "Poco not found")
endif()

add_subdirectory(core)
add_subdirectory(extras)
add_subdirectory(handlers)
add_subdirectory(tools)

## Main executable
add_executable(${PROJECT_NAME} "main.cc")
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER})
target_link_libraries(${PROJECT_NAME} PUBLIC woodpeckerserver)
