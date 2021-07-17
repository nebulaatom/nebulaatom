# Tests
add_executable(test_gen test_gen.cc)
add_test(testGen test_gen)

# Use PkgConfig
include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_search_module(GTEST REQUIRED gtest)

# Add and linking libraries for Executable
target_link_libraries(test_gen PUBLIC ${GTEST_LIBRARIES} library1 library2)