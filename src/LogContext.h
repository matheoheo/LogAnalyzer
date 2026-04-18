#pragma once
#include "LogEntry.h"
#include <unordered_map>

struct stringHash
{
	using is_transparent = void;

	[[nodiscard]] size_t operator()(const char* txt) const
	{
		return std::hash<std::string_view>{}(txt);
	}
	[[nodiscard]] size_t operator()(std::string_view txt) const
	{
		return std::hash<std::string_view>{}(txt);
	}
	[[nodiscard]] size_t operator()(const std::string& txt) const
	{
		return std::hash<std::string>{}(txt);
	}
};

struct LogContext
{
	using IndexMap = std::unordered_map<std::string_view, std::vector<size_t>, stringHash, std::equal_to<>>;
	using WordIndexMap = std::unordered_map<std::string_view, std::vector<size_t>, stringHash, std::equal_to<>>;

	const std::vector<LogEntry>& entries;
	const IndexMap& logLevelIndexes;
	const IndexMap& sourceIndexes;
	const WordIndexMap& wordIndexes;
	const std::vector<size_t>& timestampIndexes;

	LogContext(
		const std::vector<LogEntry>& entries, 
		const IndexMap& logLevelIndexes, 
		const IndexMap& sourceIndexes, 
		const WordIndexMap& wordIndexes,
		const std::vector<size_t>& timestampIndexes
	)
		:entries(entries),
		logLevelIndexes(logLevelIndexes),
		sourceIndexes(sourceIndexes),
		wordIndexes(wordIndexes),
		timestampIndexes(timestampIndexes)
	{

	}
};