
## helloworld
add_executable(helloworld "hello_world.cpp")
target_include_directories(helloworld PUBLIC ${HEADER})
target_link_libraries(helloworld PUBLIC nebulaatom)

## basicrouting
add_executable(basicrouting "basic_routing.cpp")
target_include_directories(basicrouting PUBLIC ${HEADER})
target_link_libraries(basicrouting PUBLIC nebulaatom)

## backendapi
add_executable(backendapi "backend_api.cpp")
target_include_directories(backendapi PUBLIC ${HEADER})
target_link_libraries(backendapi PUBLIC nebulaatom)

## webserver
add_executable(webserver "webserver.cpp")
target_include_directories(webserver PUBLIC ${HEADER})
target_link_libraries(webserver PUBLIC nebulaatom)
