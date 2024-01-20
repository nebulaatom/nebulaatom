# Targets

## Objects libraries

### function
add_library(function OBJECT "function.cpp")
target_include_directories(function PUBLIC ${HEADER})
set_target_properties(function PROPERTIES POSITION_INDEPENDENT_CODE 1)

### sqlaction
add_library(sqlaction OBJECT "sql_action.cpp")
target_include_directories(sqlaction PUBLIC ${HEADER})
set_target_properties(sqlaction PROPERTIES POSITION_INDEPENDENT_CODE 1)

### emailaction
add_library(emailaction OBJECT "email_action.cpp")
target_include_directories(emailaction PUBLIC ${HEADER})
set_target_properties(emailaction PROPERTIES POSITION_INDEPENDENT_CODE 1)

### action
add_library(action OBJECT "action.cpp")
target_include_directories(action PUBLIC ${HEADER})
set_target_properties(action PROPERTIES POSITION_INDEPENDENT_CODE 1)

### functionsmanager
add_library(functionsmanager OBJECT "functions_manager.cpp")
target_include_directories(functionsmanager PUBLIC ${HEADER})
set_target_properties(functionsmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
