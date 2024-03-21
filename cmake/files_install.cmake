# Install files

## Header files
install(
	FILES ${NEBULAATOM_headers_core}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/core
)
install(
	FILES ${NEBULAATOM_headers_extras}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/extras
)
install(
	FILES ${NEBULAATOM_headers_files}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/files
)
install(
	FILES ${NEBULAATOM_headers_functions}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/functions
)
install(
	FILES ${NEBULAATOM_headers_handlers}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/handlers
)
install(
	FILES ${NEBULAATOM_headers_http}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/http
)
install(
	FILES ${NEBULAATOM_headers_security}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/security
)
install(
	FILES ${NEBULAATOM_headers_tools}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/tools
)

## Libraries
install(
	TARGETS nebulaatom_static
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
if(${PROJECT_NAME}_BUILD_SHARED)
	install(
		TARGETS nebulaatom_shared
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
endif()

# Examples
if(${PROJECT_NAME}_EXAMPLES)
	install(
		TARGETS 
			helloworld
			basicrouting
			backendapi
			webserver
			sslserver
			sendemail
			avancedquery
			customerror
			fullstack
			websocket
			uploadfile
		DESTINATION 
			${CMAKE_INSTALL_LIBDIR}/nebulaatom-examples
	)
endif()

## Docs files
install(
	FILES 
		${PROJECT_SOURCE_DIR}/README.md
		${PROJECT_SOURCE_DIR}/LICENSE
	DESTINATION
		${CMAKE_INSTALL_DOCDIR}
)