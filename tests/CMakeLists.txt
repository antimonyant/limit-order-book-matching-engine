add_executable(lob_tests tests.cpp)
target_include_directories(lob_tests PRIVATE ../include)
target_link_libraries(lob_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(lob_tests)