# Testing
if(${PROJECT_NAME}_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Find packages

find_package(Poco REQUIRED COMPONENTS Foundation Net Util Data DataMySQL)
if(NOT Poco_FOUND)
    message(FATAL_ERROR "Poco not found")
endif()

find_package(MySQL)
if(NOT MySQL_FOUND)
    message(FATAL_ERROR "MySQL not found")
endif()

list(APPEND HEADER ${Poco_INCLUDE_DIRS})

include_directories(${MySQL_INCLUDE_DIRS})

add_subdirectory(core)
add_subdirectory(extras)
add_subdirectory(files)
add_subdirectory(filters)
add_subdirectory(handlers)
add_subdirectory(http)
add_subdirectory(query)
add_subdirectory(tools)

## Main executable
add_executable(${PROJECT_NAME} "main.cc")
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER})
target_link_libraries(${PROJECT_NAME} PUBLIC woodpeckerserver)
