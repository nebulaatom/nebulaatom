# Targets

## Objects libraries

### httpmethods
add_library(httpmethods OBJECT "http_methods.cc")
target_include_directories(httpmethods PUBLIC ${HEADER} ${PROJECT_BINARY_DIR})
set_target_properties(httpmethods PROPERTIES POSITION_INDEPENDENT_CODE 1)

### commonresponses
add_library(commonresponses OBJECT "common_responses.cc")
target_include_directories(commonresponses PUBLIC ${HEADER} ${PROJECT_BINARY_DIR})
set_target_properties(commonresponses PROPERTIES POSITION_INDEPENDENT_CODE 1)
