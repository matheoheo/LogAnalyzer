add_library(AnalyzerLib 
    LogEntry.cpp
    LogContext.cpp
    LogStorage.cpp
    LogManager.cpp
    LogQuery.cpp
    LogInterface.cpp
    QueryEngine.cpp)

target_compile_features(AnalyzerLib PUBLIC cxx_std_20)
target_include_directories(AnalyzerLib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR})