if(${PROJECT_NAME}_DOXYGEN)
	find_package(Doxygen)
	
	if(DOXYGEN_FOUND)
		set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
		set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

		add_custom_target(
			doc_doxygen
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM 
		)
		message("Doxygen OK")
	else()
		message(FATAL_ERROR "Doxygen not found.")
	endif()
endif()
