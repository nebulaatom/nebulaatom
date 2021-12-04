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

### route
add_library(route OBJECT "route.cc")
target_include_directories(route PUBLIC ${HEADER})
set_target_properties(route PROPERTIES POSITION_INDEPENDENT_CODE 1)

### commonresponses
add_library(commonresponses OBJECT "common_responses.cc")
target_include_directories(commonresponses PUBLIC ${HEADER})
set_target_properties(commonresponses PROPERTIES POSITION_INDEPENDENT_CODE 1)

## Shared and statics libraries

### woodpeckerserver
add_library(woodpeckerserver STATIC "woodpecker_server.cc")
target_include_directories(woodpeckerserver PUBLIC ${HEADER})
target_link_libraries(
	woodpeckerserver
	handlerfactory
	queryactions
	route
	commonresponses
	roothandler
	nullhandler
	backendhandler
	frontendhandler
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
