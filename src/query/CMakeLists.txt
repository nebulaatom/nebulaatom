# Targets

## Objects libraries

### queryactions
add_library(queryactions OBJECT "query_actions.cpp")
target_include_directories(queryactions PUBLIC ${HEADER})
set_target_properties(queryactions PROPERTIES POSITION_INDEPENDENT_CODE 1)

### databasemanager
add_library(databasemanager OBJECT "database_manager.cpp")
target_include_directories(databasemanager PUBLIC ${HEADER})
set_target_properties(databasemanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
