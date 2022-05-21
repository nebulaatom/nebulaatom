# Targets

## Objects libraries

### file
add_library(file OBJECT "file.cc")
target_include_directories(file PUBLIC ${HEADER} ${PROJECT_BINARY_DIR})
set_target_properties(file PROPERTIES POSITION_INDEPENDENT_CODE 1)

### fileproperties
add_library(fileproperties OBJECT "file_properties.cc")
target_include_directories(fileproperties PUBLIC ${HEADER} ${PROJECT_BINARY_DIR})
set_target_properties(fileproperties PROPERTIES POSITION_INDEPENDENT_CODE 1)

### filemanager
add_library(filemanager OBJECT "file_manager.cc")
target_include_directories(filemanager PUBLIC ${HEADER} ${PROJECT_BINARY_DIR})
set_target_properties(filemanager PROPERTIES POSITION_INDEPENDENT_CODE 1)
