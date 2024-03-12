
# Find packages

find_package(MySQL REQUIRED)
if(NOT MySQL_FOUND)
    message(FATAL_ERROR "MySQL not found")
endif()
list(APPEND HEADER ${MySQL_INCLUDE_DIRS})

find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL Util Data DataMySQL)
if(NOT Poco_FOUND)
    message(FATAL_ERROR "Poco not found")
endif()
list(APPEND HEADER ${Poco_INCLUDE_DIRS})

find_package(yaml-cpp REQUIRED)
if(NOT yaml-cpp_FOUND)
    message(FATAL_ERROR "yaml-cpp not found")
endif()
list(APPEND HEADER ${yaml-cpp_INCLUDE_DIRS})

include_directories(${MySQL_INCLUDE_DIRS})

add_subdirectory(core)
add_subdirectory(extras)
add_subdirectory(files)
add_subdirectory(functions)
add_subdirectory(handlers)
add_subdirectory(http)
add_subdirectory(query)
add_subdirectory(security)
add_subdirectory(tools)
add_subdirectory(examples)