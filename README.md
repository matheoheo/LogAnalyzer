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

