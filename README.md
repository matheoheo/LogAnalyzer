# LogAnalyzer
Log analysis tool written in modern C++, designed for fast querying over large datasets using custom indexing and optimized search strategies.

# Overview
Log Analyzer is console based application that allows efficient searching and filtering of structured log data.

The project focuses on performance, memory efficency and scalable querying.

Instead of scanning logs lineary, the system builds multiple indexes and uses optimized algorithms to answer queries inm milliseconds.

It supports both:
  - Command-Line Arguments (CLI)
  - Interactive query mode

# Features
  1. Fast log querying using indexed data.
  2. Multi condition filtering:
     - LogLevel (``INFO``, ``ERROR``)
     - Source (Component name)
     - Message Content
     - Time Range
  3. Interactive CLI Mode.
  4. Dynamic log file switching during run-time.

# Required Strucute
To properly read and index the data, file must keep following structure of log to be valid for this analyzer:

``[TIMESTAMP] [LOG_LEVEL] [SOURCE] Message``

# Commands
To use the application, there are 6 commands that you can use to make your filtering easy:
  ```
    file=<path>      (Required) Path to the log file
    level=<value>    Filter by log level (e.g., INFO, ERROR)
    source=<value>   Filter by source component
    msg=<value>      Search for a phrase in the message
    start=<iso>      Start time (format: YYYY-MM-DDTHH:MM:SS)
    end=<iso>        End time (format: YYYY-MM-DDTHH:MM:SS)
  ```
You can mix them however you need to.

Usage example is simple: ``file=logs.txt level=ERROR source=Payment msg=Failed start=2023-10-25T10:00:00 end=2023-10-25T10:25:00``

# Building
The project uses CMake for easy building on your own PC.
  ## Requirements
  - C++20
  - CMake
  - Git

  ## How To Build (for windows)
  1. Open cmd
  2. Navigate to where you want to clone your project (**cd C:/Projects**)
  3. Clone the repository (**git clone https://github.com/matheoheo/LogAnalyzer.git**)
  4. Enter the project folder (**cd LogAnalyzer**)
  5. Create build directory and navigate there(**mkdir build & cd build**)
  6. Configure the project (**cmake ..**)
  7. Compile the project (**cmake --build .**)
  8. Now you can navigate to proper directories and run
     - a) Tests: (**cd tests/Debug & LogAnalyzerTests.exe**)
     - b) Main project: (**cd Debug & LogAnalyzer.exe**)

# How it works?
The query engine uses **query planning strategy** combined with filtering.
  1. **Query Plan Creation**
     Each filter (level, source, timestamp, message) is transformed into ``QueryStep``
     Each step contains:
       - reference to matching log IDS (or range)
       - estimated result size
     Then all steps are sorted by ascending size (most selective first).
  2. **Base Set Selection**
     The engine starts with the most selective step:
       - If it's an index - directly copy matching IDs
       - If it's a timestamp range - construct a subset from sorted timestamps.
     If the smalles step is empty - query exits early.
  3. **Filtering**
     The engine:
       - Iterates over current result
       - Removes elements that do not match conditions
 4. **Matching Strategy**
       - Index-based filters - ``std::binary_search()`` on sorted vectors
       - Timestamp filters - direct comaprision (``<``, ``>``)
     
