# Install files

## Project
install(
	TARGETS ${PROJECT_NAME} 
	DESTINATION ${CMAKE_INSTALL_BINDIR}
)

## Header files
install(
	FILES 
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h 
		${HEADER}/cpw_woodpecker_server.h
		${HEADER}/handler_factory.h
		${HEADER}/Factory/root_handler.h
		${HEADER}/Factory/data_handler.h
		${HEADER}/Factory/null_handler.h
	DESTINATION 
		${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

## Libraries
install(
	TARGETS 
		woodpeckerserver
	DESTINATION 
		${CMAKE_INSTALL_LIBDIR}
)

## Docs files
install(
	FILES 
		${PROJECT_SOURCE_DIR}/README.md
		${PROJECT_SOURCE_DIR}/CODE_OF_CONDUCT.md
		${PROJECT_SOURCE_DIR}/LICENSE
		${PROJECT_SOURCE_DIR}/PULL_REQUEST_TEMPLATE.md
	DESTINATION
		${CMAKE_INSTALL_DOCDIR}
)

## Share files
install(
	FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
