
# Targets

## Objects libraries

### handlerfactory
add_library(handlerfactory OBJECT "handler_factory.cpp")
target_include_directories(handlerfactory PUBLIC ${HEADER})
set_target_properties(handlerfactory PROPERTIES POSITION_INDEPENDENT_CODE 1)

## Shared and statics libraries

### nebulaatom
add_library(nebulaatom STATIC "nebula_atom.cpp")
target_include_directories(nebulaatom PUBLIC ${HEADER})
target_link_libraries(
    nebulaatom
    handlerfactory
    httpmethods
    securityverification
    roothandler
    nullhandler
    backendhandler
    customhandler
    frontendhandler
    websockethandler
    route
    commonresponses
    managejson
    requestsmanager
    handlerconnection
    valuesproperties
    fileproperties
    filemanager
    file
    rowvalueformatter
    session
    sessionsmanager
    loginhandler
    databasemanager
    functionsmanager
    function
    action
    sqlaction
    emailaction
    results
    parameter
    condition
    settingsmanager
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::Util
    Poco::Data
    Poco::DataMySQL
    Poco::JSON
    yaml-cpp::yaml-cpp
)

target_include_directories(nebulaatom PUBLIC ${HEADER})
