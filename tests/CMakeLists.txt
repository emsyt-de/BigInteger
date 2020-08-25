find_package(GTest REQUIRED)

add_executable(BigIntegerTests)

target_link_libraries(BigIntegerTests PRIVATE BigInteger GTest::GTest  GTest::Main)

gtest_discover_tests(BigIntegerTests)

add_subdirectory(src)
