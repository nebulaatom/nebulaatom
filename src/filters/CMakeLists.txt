# Targets

## Objects libraries

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

### rangefilter
add_library(rangefilter STATIC "range_filter.cpp")
target_include_directories(rangefilter PUBLIC ${HEADER})
set_target_properties(rangefilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(rangefilter filter)

### listfilter
add_library(listfilter STATIC "list_filter.cpp")
target_include_directories(listfilter PUBLIC ${HEADER})
set_target_properties(listfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(listfilter filter)

### likefilter
add_library(likefilter STATIC "like_filter.cpp")
target_include_directories(likefilter PUBLIC ${HEADER})
set_target_properties(likefilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(likefilter filter)

### joinfilter
add_library(joinfilter STATIC "join_filter.cpp")
target_include_directories(joinfilter PUBLIC ${HEADER})
set_target_properties(joinfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(joinfilter filter)

### groupfilter
add_library(groupfilter STATIC "group_filter.cpp")
target_include_directories(groupfilter PUBLIC ${HEADER})
set_target_properties(groupfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(groupfilter filter)

### valuesfilter
add_library(valuesfilter STATIC "values_filter.cpp")
target_include_directories(valuesfilter PUBLIC ${HEADER})
set_target_properties(valuesfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(valuesfilter filter)

### setfilter
add_library(setfilter STATIC "set_filter.cpp")
target_include_directories(setfilter PUBLIC ${HEADER})
set_target_properties(setfilter PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(setfilter filter)

### filtersmanager
add_library(filtersmanager OBJECT "filters_manager.cpp")
target_include_directories(filtersmanager PUBLIC ${HEADER})
set_target_properties(filtersmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(filtersmanager fieldsfilter sortfilter generalfilter iqualsfilter rangefilter listfilter likefilter joinfilter groupfilter valuesfilter setfilter)
