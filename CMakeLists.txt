cmake_minimum_required(VERSION 3.22)
project(LogAnalyzerProject
    DESCRIPTION "Logs Analyzer"
    LANGUAGES CXX
    VERSION 1.0)

add_subdirectory(dependencies)
add_subdirectory(src)
add_subdirectory(tests)

add_executable(LogAnalyzer src/main.cpp)
target_link_libraries(LogAnalyzer AnalyzerLib)
