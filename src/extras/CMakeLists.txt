# Targets

## Objects libraries

### dynamicelements
add_library(dynamicelements OBJECT "dynamic_elements.cc")
target_include_directories(dynamicelements PUBLIC ${HEADER})
set_target_properties(dynamicelements PROPERTIES POSITION_INDEPENDENT_CODE 1)

### httpmethods
add_library(httpmethods OBJECT "http_methods.cc")
target_include_directories(httpmethods PUBLIC ${HEADER})
set_target_properties(httpmethods PROPERTIES POSITION_INDEPENDENT_CODE 1)

### securityverification
add_library(securityverification OBJECT "security_verification.cc")
target_include_directories(securityverification PUBLIC ${HEADER})
set_target_properties(securityverification PROPERTIES POSITION_INDEPENDENT_CODE 1)

### incorporatefilters
add_library(incorporatefilters OBJECT "incorporate_filters.cc")
target_include_directories(incorporatefilters PUBLIC ${HEADER})
set_target_properties(incorporatefilters PROPERTIES POSITION_INDEPENDENT_CODE 1)
