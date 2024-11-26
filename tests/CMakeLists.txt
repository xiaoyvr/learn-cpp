# Create the test executable
add_executable(pred_test pred_test.cc)

# Link Google Test libraries
find_package(GTest REQUIRED)
target_link_libraries(pred_test PRIVATE GTest::gtest GTest::gtest_main)

target_include_directories(pred_test PRIVATE
    ..
)

# Add the test executable to CTest
enable_testing()

include(GoogleTest)
gtest_discover_tests(pred_test)


# add_test(NAME RunAllTests COMMAND hello_test)