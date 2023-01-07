# Targets

## Objects libraries

### filters
add_library(filters OBJECT "filters.cc")
target_include_directories(filters PUBLIC ${HEADER})
set_target_properties(filters PROPERTIES POSITION_INDEPENDENT_CODE 1)

### identifyfilter
add_library(identifyfilter OBJECT "identify_filter.cc")
target_include_directories(identifyfilter PUBLIC ${HEADER})
set_target_properties(identifyfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)

### incorporatefilters
add_library(incorporatefilters OBJECT "incorporate_filters.cc")
target_include_directories(incorporatefilters PUBLIC ${HEADER})
set_target_properties(incorporatefilters PROPERTIES POSITION_INDEPENDENT_CODE 1)
