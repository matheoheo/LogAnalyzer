#include "LogQuery.h"

LogQuery& LogQuery::timeBetween(const LogEntry::TimePoint& start, const LogEntry::TimePoint& end)
{
	startTime = start;
	endTime = end;
	return *this;
}

LogQuery& LogQuery::fromTime(const LogEntry::TimePoint& start)
{
	startTime = start;
	return *this;
}

LogQuery& LogQuery::toTime(const LogEntry::TimePoint& end)
{
	endTime = end;
	return *this;
	// TODO: insert return statement here
}

LogQuery& LogQuery::levelEquals(const std::string& logLevel_)
{
	logLevel = logLevel_;
	return *this;
}

LogQuery& LogQuery::sourceEquals(const std::string& source_)
{
	source = source_;
	return *this;
}

LogQuery& LogQuery::contains(const std::string& phrase_)
{
	phrase = phrase_;
	return *this;
}
