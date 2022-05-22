# Targets

## Objects libraries

### queryactions
add_library(queryactions OBJECT "query_actions.cc")
target_include_directories(queryactions PUBLIC ${HEADER} ${PROJECT_BINARY_DIR})
set_target_properties(queryactions PROPERTIES POSITION_INDEPENDENT_CODE 1)
