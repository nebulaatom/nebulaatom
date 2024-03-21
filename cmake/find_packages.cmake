
# Add CMake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# Find MySQL
find_package(MySQL REQUIRED)
if(NOT MySQL_FOUND)
    message(FATAL_ERROR "MySQL not found")
endif()
list(APPEND HEADER ${MySQL_INCLUDE_DIRS})

# Find yaml-cpp
find_package(yaml-cpp REQUIRED)
if(NOT yaml-cpp_FOUND)
    message(FATAL_ERROR "yaml-cpp not found")
endif()
list(APPEND HEADER ${yaml-cpp_INCLUDE_DIRS})

# Find Poco
find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL Util Data DataMySQL JSON)
if(NOT Poco_FOUND)
    message(FATAL_ERROR "Poco not found")
endif()
list(APPEND HEADER ${Poco_INCLUDE_DIRS})
