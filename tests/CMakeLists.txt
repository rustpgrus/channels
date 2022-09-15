add_executable(Queue_test Queue_test.cpp)
target_link_libraries(Queue_test GTest::gtest_main)
gtest_discover_tests(Queue_test)
