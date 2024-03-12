# Install files

## Project
#install(
#	TARGETS ${PROJECT_NAME} 
#	DESTINATION ${CMAKE_INSTALL_BINDIR}
#)

## Header files
install(
	FILES 
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h 
		${PROJECT_SOURCE_DIR}/src/core/nebula_atom.h
	DESTINATION 
		${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

## Libraries
install(
	TARGETS 
		nebulaatom
	DESTINATION 
		${CMAKE_INSTALL_LIBDIR}
)

## Docs files
install(
	FILES 
		${PROJECT_SOURCE_DIR}/README.md
		${PROJECT_SOURCE_DIR}/LICENSE
	DESTINATION
		${CMAKE_INSTALL_DOCDIR}
)

## Share files
install(
	FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
