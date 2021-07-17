# Build security
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()

# Language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler options
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set up prefix
if(${PROJECT_NAME}_VOID_PREFIX)
	set(CMAKE_INSTALL_PREFIX "/tmp/${PROJECT_NAME}" CACHE PATH "..." FORCE)
else()
	if(NOT CMAKE_INSTALL_PREFIX)
		set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)
	endif()
endif()

# Set up datadir
if(${PROJECT_NAME}_APPIMAGE)
	set(${PROJECT_NAME}_DATADIR ././usr/share/${PROJECT_NAME}/)
else()
	set(${PROJECT_NAME}_DATADIR ../share/${PROJECT_NAME}/)
endif()

# Set up installer
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAYOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
include(CPack)
