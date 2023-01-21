# Targets

## Objects libraries

### dynamicelements
add_library(dynamicelements OBJECT "dynamic_elements.cpp")
target_include_directories(dynamicelements PUBLIC ${HEADER})
set_target_properties(dynamicelements PROPERTIES POSITION_INDEPENDENT_CODE 1)

### securityverification
add_library(securityverification OBJECT "security_verification.cpp")
target_include_directories(securityverification PUBLIC ${HEADER})
set_target_properties(securityverification PROPERTIES POSITION_INDEPENDENT_CODE 1)

### valuesproperties
add_library(valuesproperties OBJECT "values_properties.cpp")
target_include_directories(valuesproperties PUBLIC ${HEADER})
set_target_properties(valuesproperties PROPERTIES POSITION_INDEPENDENT_CODE 1)

### session
add_library(session OBJECT "session.cpp")
target_include_directories(session PUBLIC ${HEADER})
set_target_properties(session PROPERTIES POSITION_INDEPENDENT_CODE 1)

### staticelements
add_library(staticelements OBJECT "static_elements.cpp")
target_include_directories(staticelements PUBLIC ${HEADER})
set_target_properties(staticelements PROPERTIES POSITION_INDEPENDENT_CODE 1)
