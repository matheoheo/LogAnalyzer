#pragma once
#include "LogEntry.h"
#include <optional>

struct LogQuery
{
	std::optional<LogEntry::TimePoint> startTime;
	std::optional<LogEntry::TimePoint> endTime;
	std::optional<std::string> logLevel;
	std::optional<std::string> source;
	std::optional<std::string> phrase;

	LogQuery& timeBetween(const LogEntry::TimePoint& start, const LogEntry::TimePoint& end);
	LogQuery& fromTime(const LogEntry::TimePoint& start);
	LogQuery& toTime(const LogEntry::TimePoint& end);
	LogQuery& levelEquals(const std::string& logLevel_);
	LogQuery& sourceEquals(const std::string& source_);
	LogQuery& contains(const std::string& phrase_);
};