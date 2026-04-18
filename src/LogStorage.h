#pragma once
#include "LogContext.h"
#include <vector>
#include <filesystem>


class LogStorage
{
public:
	bool loadLogFile(const std::filesystem::path& pathToFile);
	LogContext getLogContext() const;
	bool addLogEntry(std::string_view line);
private:
	void indexEntry(const LogEntry& entry, size_t index);
	void tokenizeAndIndexMessage(std::string_view message, size_t index);
	
	void clearAll();
	bool loadRawData(const std::filesystem::path& pathToFile);
	void processLogs(std::string_view content);
private:
	std::string mRawData;
	std::vector<LogEntry> mEntries;
	LogContext::IndexMap mLogLevelIndexes;
	LogContext::IndexMap mSourceIndexes;
	LogContext::WordIndexMap mWordIndexes;
	std::vector<size_t> mTimestampIndexes;
};