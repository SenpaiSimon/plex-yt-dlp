set(TEST_NAME LoggerTest)

add_executable(
  ${TEST_NAME}
  LoggerTest.cpp
  # DUT
  ../Source/Logger.cpp
)

target_include_directories(
${TEST_NAME}
PUBLIC
  ../Source
)

target_link_libraries(
${TEST_NAME}
PUBLIC
  GTest::gmock
  GTest::gtest_main
  LoggerIf
  ThreadMock
)
    
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})