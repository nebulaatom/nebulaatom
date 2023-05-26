# Targets

## Objects libraries

### identifyfilter
add_library(identifyfilter STATIC "identify_filter.cpp")
target_include_directories(identifyfilter PUBLIC ${HEADER})
set_target_properties(identifyfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)

### incorporatefilters
add_library(incorporatefilters STATIC "incorporate_filters.cpp")
target_include_directories(incorporatefilters PUBLIC ${HEADER})
set_target_properties(incorporatefilters PROPERTIES POSITION_INDEPENDENT_CODE 1)

### filter
add_library(filter STATIC "filter.cpp")
target_include_directories(filter PUBLIC ${HEADER})
set_target_properties(filter PROPERTIES POSITION_INDEPENDENT_CODE 1)

### fieldsfilter
add_library(fieldsfilter STATIC "fields_filter.cpp")
target_include_directories(fieldsfilter PUBLIC ${HEADER})
set_target_properties(fieldsfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(fieldsfilter filter)

### sortfilter
add_library(sortfilter STATIC "sort_filter.cpp")
target_include_directories(sortfilter PUBLIC ${HEADER})
set_target_properties(sortfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(sortfilter filter)

### generalfilter
add_library(generalfilter STATIC "general_filter.cpp")
target_include_directories(generalfilter PUBLIC ${HEADER})
set_target_properties(generalfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(generalfilter filter)

### iqualsfilter
add_library(iqualsfilter STATIC "iquals_filter.cpp")
target_include_directories(iqualsfilter PUBLIC ${HEADER})
set_target_properties(iqualsfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(iqualsfilter filter)

### filtersmanager
add_library(filtersmanager OBJECT "filters_manager.cpp")
target_include_directories(filtersmanager PUBLIC ${HEADER})
set_target_properties(filtersmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(filtersmanager fieldsfilter sortfilter generalfilter iqualsfilter)
