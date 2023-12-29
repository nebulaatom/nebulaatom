# Targets

## Objects libraries

### valuesproperties
add_library(valuesproperties OBJECT "values_properties.cpp")
target_include_directories(valuesproperties PUBLIC ${HEADER})
set_target_properties(valuesproperties PROPERTIES POSITION_INDEPENDENT_CODE 1)

### session
add_library(session OBJECT "session.cpp")
target_include_directories(session PUBLIC ${HEADER})
set_target_properties(session PROPERTIES POSITION_INDEPENDENT_CODE 1)
