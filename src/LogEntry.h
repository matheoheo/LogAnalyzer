#pragma once
#include <string>
#include <chrono>

struct LogEntry
{
	using TimePoint = std::chrono::sys_seconds;

	TimePoint timestamp;
	std::string_view logLevel;
	std::string_view source;
	std::string_view message;

	static TimePoint parseTimestamp(std::string_view s);
};

inline std::ostream& operator<<(std::ostream& os, const LogEntry& entry)
{
	return os << "[" << entry.timestamp << "]" << " [" << entry.logLevel << "]" << " [" << entry.source << "]" << " " << entry.message;
}