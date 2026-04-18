#include "LogStorage.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ranges>
#include <algorithm>
#include <cctype>


bool LogStorage::loadLogFile(const std::filesystem::path& pathToFile)
{
	clearAll();
	if (!loadRawData(pathToFile))
		return false;
	processLogs(mRawData);
	
	return true;
}

LogContext LogStorage::getLogContext() const
{
	return LogContext(
		mEntries,
		mLogLevelIndexes,
		mSourceIndexes,
		mWordIndexes,
		mTimestampIndexes
	);
}

bool LogStorage::addLogEntry(std::string_view line)
{
	auto getPart = [&]() -> std::string_view
	{
		size_t s = line.find('[');
		size_t e = line.find(']', s);
		if (s == std::string_view::npos || e == std::string_view::npos) return {};
		std::string_view res = line.substr(s + 1, e - s - 1);
		line.remove_prefix(e + 1);
		return res;
	};

	auto timeStr = getPart();
	auto levelStr = getPart();
	auto sourceStr = getPart();

	if (timeStr.empty())
		return false;

	size_t msgStart = line.find_first_not_of('[');
	std::string_view msgStr = (msgStart != std::string_view::npos) ? line.substr(msgStart) : line;
	mEntries.emplace_back(LogEntry{
			.timestamp = LogEntry::parseTimestamp(timeStr),
			.logLevel = levelStr,
			.source = sourceStr,
			.message = msgStr
		});
	indexEntry(mEntries.back(), mEntries.size() - 1);
	return true;
}

void LogStorage::indexEntry(const LogEntry& entry, size_t index)
{
	auto addToMap = [&](LogContext::IndexMap& map, std::string_view key)
	{
		map[key].push_back(index);
	};

	addToMap(mLogLevelIndexes, entry.logLevel);
	addToMap(mSourceIndexes, entry.source);
	tokenizeAndIndexMessage(entry.message, index);
	mTimestampIndexes.push_back(index);
}

void LogStorage::tokenizeAndIndexMessage(std::string_view message, size_t index)
{
	size_t start = 0;
	size_t end = message.find(' ');

	while (start < message.size())
	{
		std::string_view thisToken = message.substr(start, end - start);

		while (!thisToken.empty() && std::ispunct(static_cast<unsigned char>(thisToken.back()))) 
			thisToken.remove_suffix(1);

		if (thisToken.size() >= 3)
		{
			auto [it, inserted] = mWordIndexes.try_emplace(thisToken);
			it->second.push_back(index);
		}

		if (end == std::string_view::npos) break;

		start = end + 1;
		end = message.find(' ', start);
	}
}

void LogStorage::clearAll()
{
	mEntries.clear();
	mTimestampIndexes.clear();
	mSourceIndexes.clear();
	mLogLevelIndexes.clear();
	mRawData.clear();
}

bool LogStorage::loadRawData(const std::filesystem::path& pathToFile)
{
	std::ifstream file(pathToFile, std::ios::binary | std::ios::ate);
	if (!file)
	{
		std::cout << "Failed to open: " << pathToFile.string() << '\n';
		return false;
	}
	auto size = file.tellg();
	mRawData.resize(size);
	file.seekg(0);
	file.read(mRawData.data(), size);

	return true;
}

void LogStorage::processLogs(std::string_view content)
{
	size_t start = 0;
	size_t end = content.find('\n');

	while (end != std::string_view::npos)
	{
		std::string_view line = content.substr(start, end - start);

		if (!line.empty() && line.back() == '\r')
			line.remove_suffix(1);

		if (!line.empty())
			addLogEntry(line);

		start = end + 1;
		end = content.find('\n', start);
	}

	if (start < content.size())
	{
		std::string_view lastLine = content.substr(start);
		if (!lastLine.empty()) addLogEntry(lastLine);
	}
}
