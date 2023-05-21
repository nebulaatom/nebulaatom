# Targets

## Objects libraries

### filtersmanager
add_library(filtersmanager OBJECT "filters_manager.cpp")
target_include_directories(filtersmanager PUBLIC ${HEADER})
set_target_properties(filtersmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)

### identifyfilter
add_library(identifyfilter OBJECT "identify_filter.cpp")
target_include_directories(identifyfilter PUBLIC ${HEADER})
set_target_properties(identifyfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)

### incorporatefilters
add_library(incorporatefilters OBJECT "incorporate_filters.cpp")
target_include_directories(incorporatefilters PUBLIC ${HEADER})
set_target_properties(incorporatefilters PROPERTIES POSITION_INDEPENDENT_CODE 1)

### filter
add_library(filter OBJECT "filter.cpp")
target_include_directories(filter PUBLIC ${HEADER})
set_target_properties(filter PROPERTIES POSITION_INDEPENDENT_CODE 1)

### fieldsfilter
add_library(fieldsfilter OBJECT "fields_filter.cpp")
target_include_directories(fieldsfilter PUBLIC ${HEADER})
set_target_properties(fieldsfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
