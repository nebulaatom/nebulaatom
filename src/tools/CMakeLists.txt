# Targets

## Objects libraries

### route
add_library(route OBJECT "route.cc")
target_include_directories(route PUBLIC ${HEADER})
set_target_properties(route PROPERTIES POSITION_INDEPENDENT_CODE 1)

### requestsmanager
add_library(requestsmanager OBJECT "requests_manager.cc")
target_include_directories(requestsmanager PUBLIC ${HEADER})
set_target_properties(requestsmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)

### handlerconnection
add_library(handlerconnection OBJECT "handler_connection.cc")
target_include_directories(handlerconnection PUBLIC ${HEADER})
set_target_properties(handlerconnection PROPERTIES POSITION_INDEPENDENT_CODE 1)

### rowvalueformatter
add_library(rowvalueformatter OBJECT "row_value_formatter.cc")
target_include_directories(rowvalueformatter PUBLIC ${HEADER})
set_target_properties(rowvalueformatter PROPERTIES POSITION_INDEPENDENT_CODE 1)

### managejson
add_library(managejson OBJECT "manage_json.cc")
target_include_directories(managejson PUBLIC ${HEADER})
set_target_properties(managejson PROPERTIES POSITION_INDEPENDENT_CODE 1)
