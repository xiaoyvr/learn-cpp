# Create the test executable
add_executable(hello_test hello_test.cc)

# Link Google Test libraries
find_package(GTest REQUIRED)
target_link_libraries(hello_test PRIVATE GTest::gtest GTest::gtest_main)

target_include_directories(hello_test PRIVATE
    ..
)

# Add the test executable to CTest
enable_testing()
add_test(NAME RunAllTests COMMAND hello_test)