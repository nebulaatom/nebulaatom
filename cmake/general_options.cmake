# General options

option(${PROJECT_NAME}_TESTING "Enable testing with Google Test" OFF)
option(${PROJECT_NAME}_CONAN "Enable Conan package manager" OFF)
option(${PROJECT_NAME}_DOXYGEN "Enable Doxygen documentation" OFF)
option(${PROJECT_NAME}_CCACHE "Enable ccache to speed up the build time" OFF)
option(${PROJECT_NAME}_CPPCHECK "Enable cppcheck to verify the sources syntax" OFF)
option(${PROJECT_NAME}_VOID_PREFIX "Void prefix to install the project on CMake binary dir" OFF)
option(${PROJECT_NAME}_APPIMAGE "Settings to create an AppImage" OFF)

# Handling options

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
