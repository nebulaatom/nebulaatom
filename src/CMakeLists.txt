add_subdirectory(Factory)

# Find packages
include(${PROJECT_SOURCE_DIR}/cmake/FindMySQL.cmake)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util Data DataMySQL)
if(NOT Poco_FOUND)
	message(FATAL_ERROR "Poco not found")
endif()


# Targets

## Objects libraries

### handlerfactory
add_library(handlerfactory OBJECT "handler_factory.cc")
target_include_directories(handlerfactory PUBLIC ${HEADER})
set_target_properties(handlerfactory PROPERTIES POSITION_INDEPENDENT_CODE 1)

### queryactions
add_library(queryactions OBJECT "query_actions.cc")
target_include_directories(queryactions PUBLIC ${HEADER})
set_target_properties(queryactions PROPERTIES POSITION_INDEPENDENT_CODE 1)

### errorreport
add_library(errorreport OBJECT "error_report.cc")
target_include_directories(errorreport PUBLIC ${HEADER})
set_target_properties(errorreport PROPERTIES POSITION_INDEPENDENT_CODE 1)

## Shared and statics libraries

### woodpeckerserver
add_library(woodpeckerserver STATIC "cpw_woodpecker_server.cc")
target_include_directories(woodpeckerserver PUBLIC ${HEADER})
target_link_libraries(
	woodpeckerserver
	handlerfactory
	queryactions
	errorreport
	roothandler
	nullhandler
	businesshandler
	webhandler
	Poco::Foundation
	Poco::Net
	Poco::Util
	Poco::Data
	Poco::Data
	Poco::DataMySQL
	Poco::JSON
)

## Main executable
add_executable(${PROJECT_NAME} "main.cc")
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER})
target_link_libraries(${PROJECT_NAME} PUBLIC woodpeckerserver)
