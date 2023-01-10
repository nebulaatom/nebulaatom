# Tests
add_executable(test_gen test_gen.cc)
add_test(TestGen test_gen)

# Find GTest
find_package(GTest REQUIRED)

# Add and linking libraries for Executable
target_include_directories(test_gen PUBLIC ${HEADER} ${GTest_INCLUDE_DIRS})
target_link_libraries(test_gen PUBLIC GTest::gtest woodpeckerserver)
