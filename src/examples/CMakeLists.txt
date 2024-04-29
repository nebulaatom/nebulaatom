
## helloworld
add_executable(helloworld "hello_world.cpp")
target_include_directories(helloworld PUBLIC ${HEADER})
target_link_libraries(helloworld PUBLIC nebulaatom_static)

## basicrouting
add_executable(basicrouting "basic_routing.cpp")
target_include_directories(basicrouting PUBLIC ${HEADER})
target_link_libraries(basicrouting PUBLIC nebulaatom_static)

## backendapi
add_executable(backendapi "backend_api.cpp")
target_include_directories(backendapi PUBLIC ${HEADER})
target_link_libraries(backendapi PUBLIC nebulaatom_static)

## webserver
add_executable(webserver "webserver.cpp")
target_include_directories(webserver PUBLIC ${HEADER})
target_link_libraries(webserver PUBLIC nebulaatom_static)

## sslserver
add_executable(sslserver "ssl_server.cpp")
target_include_directories(sslserver PUBLIC ${HEADER})
target_link_libraries(sslserver PUBLIC nebulaatom_static)

## sendemail
add_executable(sendemail "send_email.cpp")
target_include_directories(sendemail PUBLIC ${HEADER})
target_link_libraries(sendemail PUBLIC nebulaatom_static)

## avancedquery
add_executable(avancedquery "avanced_query.cpp")
target_include_directories(avancedquery PUBLIC ${HEADER})
target_link_libraries(avancedquery PUBLIC nebulaatom_static)

## customerror
add_executable(customerror "custom_error.cpp")
target_include_directories(customerror PUBLIC ${HEADER})
target_link_libraries(customerror PUBLIC nebulaatom_static)

## fullstack
add_executable(fullstack "fullstack.cpp")
target_include_directories(fullstack PUBLIC ${HEADER})
target_link_libraries(fullstack PUBLIC nebulaatom_static)

## websocket
add_executable(websocket "websocket.cpp")
target_include_directories(websocket PUBLIC ${HEADER})
target_link_libraries(websocket PUBLIC nebulaatom_static)

## uploadfile
add_executable(uploadfile "upload_file.cpp")
target_include_directories(uploadfile PUBLIC ${HEADER})
target_link_libraries(uploadfile PUBLIC nebulaatom_static)

## setupheaders
add_executable(setupheaders "setup_headers.cpp")
target_include_directories(setupheaders PUBLIC ${HEADER})
target_link_libraries(setupheaders PUBLIC nebulaatom_static)

## clientrequest
add_executable(clientrequest "client_request.cpp")
target_include_directories(clientrequest PUBLIC ${HEADER})
target_link_libraries(clientrequest PUBLIC nebulaatom_static)

## usingmethods
add_executable(usingmethods "using_methods.cpp")
target_include_directories(usingmethods PUBLIC ${HEADER})
target_link_libraries(usingmethods PUBLIC nebulaatom_static)

## postapi
add_executable(postapi "post_api.cpp")
target_include_directories(postapi PUBLIC ${HEADER})
target_link_libraries(postapi PUBLIC nebulaatom_static)
