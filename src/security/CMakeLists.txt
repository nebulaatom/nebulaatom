# Targets

## Objects libraries

### user
add_library(user STATIC "user.cpp")
target_include_directories(user PUBLIC ${HEADER})
set_target_properties(user PROPERTIES POSITION_INDEPENDENT_CODE 1)

### usersmanager
add_library(usersmanager STATIC "users_manager.cpp")
target_include_directories(usersmanager PUBLIC ${HEADER})
set_target_properties(usersmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(usersmanager user)

### permission
add_library(permission STATIC "permission.cpp")
target_include_directories(permission PUBLIC ${HEADER})
set_target_properties(permission PROPERTIES POSITION_INDEPENDENT_CODE 1)

### permissionsmanager
add_library(permissionsmanager STATIC "permissions_manager.cpp")
target_include_directories(permissionsmanager PUBLIC ${HEADER})
set_target_properties(permissionsmanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(permissionsmanager permission)

### securityverification
add_library(securityverification STATIC "security_verification.cpp")
target_include_directories(securityverification PUBLIC ${HEADER})
set_target_properties(securityverification PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(securityverification permission permissionsmanager usersmanager)
