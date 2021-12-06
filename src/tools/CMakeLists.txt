# Targets

## Objects libraries

### route
add_library(route OBJECT "route.cc")
target_include_directories(route PUBLIC ${HEADER})
set_target_properties(route PROPERTIES POSITION_INDEPENDENT_CODE 1)

### commonresponses
add_library(commonresponses OBJECT "common_responses.cc")
target_include_directories(commonresponses PUBLIC ${HEADER})
set_target_properties(commonresponses PROPERTIES POSITION_INDEPENDENT_CODE 1)

### filters
add_library(filters OBJECT "filters.cc")
target_include_directories(filters PUBLIC ${HEADER})
set_target_properties(filters PROPERTIES POSITION_INDEPENDENT_CODE 1)

### managejson
add_library(managejson OBJECT "manage_json.cc")
target_include_directories(managejson PUBLIC ${HEADER})
set_target_properties(managejson PROPERTIES POSITION_INDEPENDENT_CODE 1)
