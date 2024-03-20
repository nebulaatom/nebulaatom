
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

# Sources
set(NEBULAATOM_SOURCES
    ${PROJECT_SOURCE_DIR}/src/core/nebula_atom.cpp
    ${PROJECT_SOURCE_DIR}/src/core/handler_factory.cpp
    ${PROJECT_SOURCE_DIR}/src/extras/session.cpp
    ${PROJECT_SOURCE_DIR}/src/extras/values_properties.cpp
    ${PROJECT_SOURCE_DIR}/src/files/file_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/files/file_properties.cpp
    ${PROJECT_SOURCE_DIR}/src/files/file.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/action.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/email_action.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/sql_action.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/function.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/functions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/root_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/backend_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/frontend_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/login_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/null_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/custom_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/websocket_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/http/common_responses.cpp
    ${PROJECT_SOURCE_DIR}/src/http/http_methods.cpp
    ${PROJECT_SOURCE_DIR}/src/query/database_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/query/condition.cpp
    ${PROJECT_SOURCE_DIR}/src/query/parameter.cpp
    ${PROJECT_SOURCE_DIR}/src/query/results.cpp
    ${PROJECT_SOURCE_DIR}/src/security/users_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/security/user.cpp
    ${PROJECT_SOURCE_DIR}/src/security/permissions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/security/permission.cpp
    ${PROJECT_SOURCE_DIR}/src/security/security_verification.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/handler_connection.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/manage_json.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/requests_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/route.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/row_value_formatter.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/sessions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/settings_manager.cpp
)

# Headers
set(NEBULAATOM_headers_core
    ${PROJECT_SOURCE_DIR}/src/core/nebula_atom.h
    ${PROJECT_SOURCE_DIR}/src/core/handler_factory.h
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h
)
set(NEBULAATOM_headers_extras
    ${PROJECT_SOURCE_DIR}/src/extras/session.h
    ${PROJECT_SOURCE_DIR}/src/extras/values_properties.h
)
set(NEBULAATOM_headers_files
    ${PROJECT_SOURCE_DIR}/src/files/file_manager.h
    ${PROJECT_SOURCE_DIR}/src/files/file_properties.h
    ${PROJECT_SOURCE_DIR}/src/files/file.h
)
set(NEBULAATOM_headers_functions
    ${PROJECT_SOURCE_DIR}/src/functions/action.h
    ${PROJECT_SOURCE_DIR}/src/functions/email_action.h
    ${PROJECT_SOURCE_DIR}/src/functions/sql_action.h
    ${PROJECT_SOURCE_DIR}/src/functions/function.h
    ${PROJECT_SOURCE_DIR}/src/functions/functions_manager.h
)
set(NEBULAATOM_headers_handlers
    ${PROJECT_SOURCE_DIR}/src/handlers/root_handler.h
    ${PROJECT_SOURCE_DIR}/src/handlers/backend_handler.h
    ${PROJECT_SOURCE_DIR}/src/handlers/frontend_handler.h
    ${PROJECT_SOURCE_DIR}/src/handlers/login_handler.h
    ${PROJECT_SOURCE_DIR}/src/handlers/null_handler.h
    ${PROJECT_SOURCE_DIR}/src/handlers/custom_handler.h
    ${PROJECT_SOURCE_DIR}/src/handlers/websocket_handler.h
)
set(NEBULAATOM_headers_http
    ${PROJECT_SOURCE_DIR}/src/http/common_responses.h
    ${PROJECT_SOURCE_DIR}/src/http/http_methods.h
)
set(NEBULAATOM_headers_query
    ${PROJECT_SOURCE_DIR}/src/query/database_manager.h
    ${PROJECT_SOURCE_DIR}/src/query/condition.h
    ${PROJECT_SOURCE_DIR}/src/query/parameter.h
    ${PROJECT_SOURCE_DIR}/src/query/results.h
)
set(NEBULAATOM_headers_security
    ${PROJECT_SOURCE_DIR}/src/security/users_manager.h
    ${PROJECT_SOURCE_DIR}/src/security/user.h
    ${PROJECT_SOURCE_DIR}/src/security/permissions_manager.h
    ${PROJECT_SOURCE_DIR}/src/security/permission.h
    ${PROJECT_SOURCE_DIR}/src/security/security_verification.h
)
set(NEBULAATOM_headers_tools
    ${PROJECT_SOURCE_DIR}/src/tools/handler_connection.h
    ${PROJECT_SOURCE_DIR}/src/tools/manage_json.h
    ${PROJECT_SOURCE_DIR}/src/tools/requests_manager.h
    ${PROJECT_SOURCE_DIR}/src/tools/route.h
    ${PROJECT_SOURCE_DIR}/src/tools/row_value_formatter.h
    ${PROJECT_SOURCE_DIR}/src/tools/sessions_manager.h
    ${PROJECT_SOURCE_DIR}/src/tools/settings_manager.h
)
set(HEADER
    ${HEADER}
    ${NEBULAATOM_headers_core}
    ${NEBULAATOM_headers_extras}
    ${NEBULAATOM_headers_files}
    ${NEBULAATOM_headers_functions}
    ${NEBULAATOM_headers_handlers}
    ${NEBULAATOM_headers_http}
    ${NEBULAATOM_headers_query}
    ${NEBULAATOM_headers_security}
    ${NEBULAATOM_headers_tools}
)

# Library
add_library(nebulaatom_static STATIC ${NEBULAATOM_SOURCES})
add_library(nebulaatom_shared SHARED ${NEBULAATOM_SOURCES})

# Include directories
target_include_directories(nebulaatom_shared PUBLIC ${HEADER})
target_include_directories(nebulaatom_static PUBLIC ${HEADER})

# Link libraries
target_link_libraries(nebulaatom_static
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::Util
    Poco::Data
    Poco::DataMySQL
    Poco::JSON
    yaml-cpp::yaml-cpp
)
target_link_libraries(nebulaatom_shared
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::Util
    Poco::Data
    Poco::DataMySQL
    Poco::JSON
    yaml-cpp::yaml-cpp
)

# Add examples
if(${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(examples)
endif()

# Install
include(${PROJECT_SOURCE_DIR}/cmake/files_install.cmake)
