
## Main executable
add_executable(helloworld "hello_world.cpp")
target_include_directories(helloworld PUBLIC ${HEADER})
target_link_libraries(helloworld PUBLIC woodpeckerserver)
