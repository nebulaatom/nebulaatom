# Targets

## Objects libraries

### parameter
add_library(parameter OBJECT "parameter.cpp")
target_include_directories(parameter PUBLIC ${HEADER})
set_target_properties(parameter PROPERTIES POSITION_INDEPENDENT_CODE 1)

### results
add_library(results OBJECT "results.cpp")
target_include_directories(results PUBLIC ${HEADER})
set_target_properties(results PROPERTIES POSITION_INDEPENDENT_CODE 1)

### condition
add_library(condition OBJECT "condition.cpp")
target_include_directories(condition PUBLIC ${HEADER})
set_target_properties(condition PROPERTIES POSITION_INDEPENDENT_CODE 1)

### databasemanager
add_library(databasemanager OBJECT "database_manager.cpp")
target_include_directories(databasemanager PUBLIC ${HEADER})
set_target_properties(databasemanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
