#pragma once
#include "LogQuery.h"
#include <filesystem>

class LogInterface
{
public:
	struct ParserResult
	{
		std::filesystem::path pathToFile;
		LogQuery q;
	};
	static ParserResult parseArgs(int argc, char* argv[]);
	static ParserResult parseLine(const std::string& line);
private:
	static ParserResult buildQuery(const std::vector<std::string>& tokens);
	static bool isValidTimestamp(const std::string& str);
};