
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

## sslserver
add_executable(sslserver "ssl_server.cpp")
target_include_directories(sslserver PUBLIC ${HEADER})
target_link_libraries(sslserver PUBLIC nebulaatom)

## sendemail
add_executable(sendemail "send_email.cpp")
target_include_directories(sendemail PUBLIC ${HEADER})
target_link_libraries(sendemail PUBLIC nebulaatom)

## avancedquery
add_executable(avancedquery "avanced_query.cpp")
target_include_directories(avancedquery PUBLIC ${HEADER})
target_link_libraries(avancedquery PUBLIC nebulaatom)

## customerror
add_executable(customerror "custom_error.cpp")
target_include_directories(customerror PUBLIC ${HEADER})
target_link_libraries(customerror PUBLIC nebulaatom)
