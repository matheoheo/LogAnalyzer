#pragma once
#include "LogStorage.h"
#include "QueryEngine.h"

class LogManager
{
public:
	bool loadLogFile(const std::filesystem::path& path);
	void printQueredData(const LogQuery& query) const;
private:
	LogStorage mStorage;
};