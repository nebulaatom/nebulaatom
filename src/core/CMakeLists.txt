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

## Shared and statics libraries

### woodpeckerserver
add_library(woodpeckerserver STATIC "woodpecker_server.cc")
target_include_directories(woodpeckerserver PUBLIC ${HEADER})
target_link_libraries(
	woodpeckerserver
	handlerfactory
	queryactions
	dynamicelements
	httpmethods
	securityverification
	roothandler
	nullhandler
	backendhandler
	frontendhandler
	route
	commonresponses
	filters
	managejson
	requestsmanager
	incorporatefilters
	Poco::Foundation
	Poco::Net
	Poco::Util
	Poco::Data
	Poco::DataMySQL
	Poco::JSON
)
