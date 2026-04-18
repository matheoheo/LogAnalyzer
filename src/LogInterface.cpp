#include "LogInterface.h"
#include "LogEntry.h"
#include <chrono>
#include <sstream>
#include <iostream>

LogInterface::ParserResult LogInterface::parseArgs(int argc, char* argv[])
{
    std::vector<std::string> tokens;
    for (int i = 1; i < argc; ++i)
        tokens.push_back(argv[i]);

    return buildQuery(tokens);
}

LogInterface::ParserResult LogInterface::parseLine(const std::string& line)
{
    std::vector<std::string> tokens;
    std::stringstream sstr(line);
    std::string token;
    while (sstr >> token) 
        tokens.push_back(token);
    return buildQuery(tokens);
}

LogInterface::ParserResult LogInterface::buildQuery(const std::vector<std::string>& tokens)
{
    ParserResult result;
    for (const auto& token : tokens)
    {
        size_t sep = token.find('=');
        if (sep == std::string::npos || sep == token.size() - 1)
            continue;
        std::string key = token.substr(0, sep);
        std::string val = token.substr(sep + 1);

        if (key == "file") result.pathToFile = val;
        else if (key == "level") result.q.logLevel = val;
        else if (key == "source") result.q.source = val;
        else if (key == "msg") result.q.phrase = val;
        else if (key == "start")
        {
            if (isValidTimestamp(val))
                result.q.startTime = LogEntry::parseTimestamp(val);
            else
                std::cout << "Invalid start timestamp\n";
        }
        else if (key == "end")
        {
            if (isValidTimestamp(val))
                result.q.endTime = LogEntry::parseTimestamp(val);
            else
                std::cout << "Invalid end timestamp\n";
        }
    }
    if (result.q.startTime && result.q.endTime && result.q.startTime > result.q.endTime)
        std::cout << "Invalid time: start shouldn't be greater than end.\n";

    return result;
}

bool LogInterface::isValidTimestamp(const std::string& str)
{
    if (str.empty()) return false;

    std::istringstream ss{ str };
    LogEntry::TimePoint tp;
    ss >> std::chrono::parse("%FT%T", tp);

    return !ss.fail();
}
