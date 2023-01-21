# Targets

## Objects libraries

### roothandler
add_library(roothandler OBJECT "root_handler.cpp")
target_include_directories(roothandler PUBLIC ${HEADER})
set_target_properties(roothandler PROPERTIES POSITION_INDEPENDENT_CODE 1)

### nullhandler
add_library(nullhandler OBJECT "null_handler.cpp")
target_include_directories(nullhandler PUBLIC ${HEADER})
set_target_properties(nullhandler PROPERTIES POSITION_INDEPENDENT_CODE 1)

### backendhandler
add_library(backendhandler OBJECT "backend_handler.cpp")
target_include_directories(backendhandler PUBLIC ${HEADER})
set_target_properties(backendhandler PROPERTIES POSITION_INDEPENDENT_CODE 1)

### frontendhandler
add_library(frontendhandler OBJECT "frontend_handler.cpp")
target_include_directories(frontendhandler PUBLIC ${HEADER})
set_target_properties(frontendhandler PROPERTIES POSITION_INDEPENDENT_CODE 1)

### loginhandler
add_library(loginhandler OBJECT "login_handler.cpp")
target_include_directories(loginhandler PUBLIC ${HEADER})
set_target_properties(loginhandler PROPERTIES POSITION_INDEPENDENT_CODE 1)
