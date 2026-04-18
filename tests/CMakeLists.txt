enable_testing()

add_executable(LogAnalyzerTests tests.cpp)

target_link_libraries(LogAnalyzerTests
    AnalyzerLib
    gtest_main)
    
include(GoogleTest)

gtest_discover_tests(LogAnalyzerTests)