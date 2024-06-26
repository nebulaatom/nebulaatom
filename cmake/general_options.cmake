# General options

option(${PROJECT_NAME}_CCACHE "Enable ccache to speed up the build time" OFF)
option(${PROJECT_NAME}_CPPCHECK "Enable cppcheck to verify the sources syntax" OFF)
option(${PROJECT_NAME}_DOXYGEN "Enable Doxygen documentation" OFF)
option(${PROJECT_NAME}_EXAMPLES "Enable examples generation" ON)
option(${PROJECT_NAME}_BUILD_SHARED "Enable build shared library" ON)

# Handling options

## ccache

if(${PROJECT_NAME}_CCACHE)
	find_program(CCACHE_FOUND ccache)

	if(CCACHE_FOUND)
		set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
		set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
		message("ccache OK")
	else()
		message(FATAL_ERROR "ccache not found.")
	endif()
endif()

## cppcheck

if(${PROJECT_NAME}_CPPCHECK)
	find_program(CPPCHECK_FOUND cppcheck)

	if(CPPCHECK_FOUND)
		set(CMAKE_CXX_CPPCHECK "cppcheck")
		list(
			APPEND CMAKE_CXX_CPPCHECK
			"--language=c++"
			"--enable=warning,style"
			"--inconclusive"
			"--inline-suppr"
		)
		message("CppCheck OK")
	else()
		message(FATAL_ERROR "CppCheck not found.")
	endif()
endif()

## doxygen

if(${PROJECT_NAME}_DOXYGEN)
	find_package(Doxygen)

	if(DOXYGEN_FOUND)
		set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
		set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		configure_file(${PROJECT_SOURCE_DIR}/docs/header.html.in ${PROJECT_BINARY_DIR}/header.html @ONLY)
		configure_file(${PROJECT_SOURCE_DIR}/docs/footer.html.in ${PROJECT_BINARY_DIR}/footer.html @ONLY)
		configure_file(${PROJECT_SOURCE_DIR}/docs/styles.css.in ${PROJECT_BINARY_DIR}/styles.css @ONLY)

		message("Doxygen OK")
	else()
		message(FATAL_ERROR "Doxygen not found.")
	endif()
endif()
