# do this after all install(...) commands so that all targets are finalized. 
# Essentially, the last thing included at the end of the top-level CMakeLists.txt

set(CPACK_PACKAGE_NAME libnebulaatom)
set(CPACK_PACKAGE_VENDOR "NebulaCSF")
set(CPACK_GENERATOR "DEB")
set(CPACK_SOURCE_GENERATOR "DEB")
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR}/cpack)

set(CPACK_SOURCE_IGNORE_FILES .git/ .github/ .vscode/ .gitignore
${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR}/
)

# Set up installer
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpoco-dev (>= 1.10.0), libyaml-cpp-dev (>= 0.6.3), libssl-dev (>= 1.1.1), libmariadb-dev (>= 10.5.23)")
set(CPACK_PACKAGE_DESCRIPTION "C++ Web Framework for building high-performance APIs, supporting scalable architecture and modern web technologies")
set(CPACK_PACKAGE_CONTACT "Jose Rivas josefelixrc7@gmail.com")

include(CPack)