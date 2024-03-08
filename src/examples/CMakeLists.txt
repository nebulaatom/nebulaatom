
## helloworld
add_executable(helloworld "hello_world.cpp")
target_include_directories(helloworld PUBLIC ${HEADER})
target_link_libraries(helloworld PUBLIC woodpeckerserver)

## basicrouting
add_executable(basicrouting "basic_routing.cpp")
target_include_directories(basicrouting PUBLIC ${HEADER})
target_link_libraries(basicrouting PUBLIC woodpeckerserver)

## backendapi
add_executable(backendapi "backend_api.cpp")
target_include_directories(backendapi PUBLIC ${HEADER})
target_link_libraries(backendapi PUBLIC woodpeckerserver)
