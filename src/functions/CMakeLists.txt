# Targets

## Objects libraries

### function
add_library(function STATIC "function.cpp")
target_include_directories(function PUBLIC ${HEADER})
set_target_properties(function PROPERTIES POSITION_INDEPENDENT_CODE 1)

### functionsmanager
add_library(functionsmanager OBJECT "functions_manager.cpp")
target_include_directories(functionsmanager PUBLIC ${HEADER})
set_target_properties(functionsmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(functionsmanager function)
