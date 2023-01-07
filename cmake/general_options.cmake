# General options

option(${PROJECT_NAME}_TESTING "Enable testing with Google Test" OFF)
option(${PROJECT_NAME}_CCACHE "Enable ccache to speed up the build time" OFF)
option(${PROJECT_NAME}_CPPCHECK "Enable cppcheck to verify the sources syntax" OFF)
option(${PROJECT_NAME}_DOXYGEN "Enable Doxygen documentation" OFF)
option(${PROJECT_NAME}_VOID_PREFIX "Void prefix to install the project on /tmp" OFF)
option(${PROJECT_NAME}_APPIMAGE "Settings to create an AppImage" OFF)

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

# Void Prefix

if(${PROJECT_NAME}_VOID_PREFIX)
	set(CMAKE_INSTALL_PREFIX "/tmp/${PROJECT_NAME}" CACHE PATH "..." FORCE)
else()
	if(NOT CMAKE_INSTALL_PREFIX)
		set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)
	endif()
endif()

# appimage

if(${PROJECT_NAME}_APPIMAGE)
	set(${PROJECT_NAME}_DATADIR ././usr/share/${PROJECT_NAME}/)
else()
	set(${PROJECT_NAME}_DATADIR ../share/${PROJECT_NAME}/)
endif()
