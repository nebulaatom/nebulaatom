
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
    ${PROJECT_SOURCE_DIR}/src/functions/function.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/functions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/root_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/backend_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/frontend_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/login_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/null_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/custom_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/websocket_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/http/client.cpp
    ${PROJECT_SOURCE_DIR}/src/http/common_responses.cpp
    ${PROJECT_SOURCE_DIR}/src/http/methods.cpp
    ${PROJECT_SOURCE_DIR}/src/http/request.cpp
    ${PROJECT_SOURCE_DIR}/src/query/database_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/query/condition.cpp
    ${PROJECT_SOURCE_DIR}/src/query/parameter.cpp
    ${PROJECT_SOURCE_DIR}/src/query/results.cpp
    ${PROJECT_SOURCE_DIR}/src/query/row.cpp
    ${PROJECT_SOURCE_DIR}/src/query/field.cpp
    ${PROJECT_SOURCE_DIR}/src/security/users_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/security/user.cpp
    ${PROJECT_SOURCE_DIR}/src/security/permissions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/security/permission.cpp
    ${PROJECT_SOURCE_DIR}/src/security/security_verification.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/handler_connection.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/manage_json.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/route.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/dvalue.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/sessions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/settings_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/output_logger.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/email.cpp
)

# Headers
set(NEBULAATOM_headers_core
    ${PROJECT_SOURCE_DIR}/src/core/nebula_atom.h
    ${PROJECT_SOURCE_DIR}/src/core/handler_factory.h
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
    ${PROJECT_SOURCE_DIR}/src/http/client.h
    ${PROJECT_SOURCE_DIR}/src/http/common_responses.h
    ${PROJECT_SOURCE_DIR}/src/http/methods.h
    ${PROJECT_SOURCE_DIR}/src/http/request.h
)
set(NEBULAATOM_headers_query
    ${PROJECT_SOURCE_DIR}/src/query/database_manager.h
    ${PROJECT_SOURCE_DIR}/src/query/condition.h
    ${PROJECT_SOURCE_DIR}/src/query/parameter.h
    ${PROJECT_SOURCE_DIR}/src/query/results.h
    ${PROJECT_SOURCE_DIR}/src/query/row.h
    ${PROJECT_SOURCE_DIR}/src/query/field.h
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
    ${PROJECT_SOURCE_DIR}/src/tools/manage_json.cmake_host_system_information()
    ${PROJECT_SOURCE_DIR}/src/tools/route.h
    ${PROJECT_SOURCE_DIR}/src/tools/dvalue.h
    ${PROJECT_SOURCE_DIR}/src/tools/sessions_manager.h
    ${PROJECT_SOURCE_DIR}/src/tools/settings_manager.h
    ${PROJECT_SOURCE_DIR}/src/tools/output_logger.h
    ${PROJECT_SOURCE_DIR}/src/tools/email.h
)
set(HEADER
    ${HEADER}
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/extras
    ${PROJECT_SOURCE_DIR}/files
    ${PROJECT_SOURCE_DIR}/functions
    ${PROJECT_SOURCE_DIR}/handlers
    ${PROJECT_SOURCE_DIR}/http
    ${PROJECT_SOURCE_DIR}/query
    ${PROJECT_SOURCE_DIR}/security
    ${PROJECT_SOURCE_DIR}/tools
)

# Dependencies
set(LIB_DEPENDECIES
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::Util
    Poco::Data
    Poco::DataMySQL
    Poco::JSON
    yaml-cpp::yaml-cpp
)

# nebulaatom OBJECT
add_library(nebulaatom OBJECT ${NEBULAATOM_SOURCES})
target_include_directories(nebulaatom PUBLIC ${HEADER})
target_link_libraries(nebulaatom ${LIB_DEPENDECIES})
set_target_properties(nebulaatom PROPERTIES POSITION_INDEPENDENT_CODE 1)

# Libraries
add_library(nebulaatom_static STATIC $<TARGET_OBJECTS:nebulaatom>)
target_link_libraries(nebulaatom_static ${LIB_DEPENDECIES})
set_target_properties(nebulaatom_static PROPERTIES OUTPUT_NAME nebulaatom)

if(${PROJECT_NAME}_BUILD_SHARED)
    add_library(nebulaatom_shared SHARED $<TARGET_OBJECTS:nebulaatom>)
    target_link_libraries(nebulaatom_shared ${LIB_DEPENDECIES})
    set_target_properties(nebulaatom_shared PROPERTIES OUTPUT_NAME nebulaatom)
endif()

# Add examples
if(${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(examples)
endif()

# Install
include(${PROJECT_SOURCE_DIR}/cmake/files_install.cmake)
