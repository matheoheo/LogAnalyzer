#include "LogManager.h"
#include <iostream>

bool LogManager::loadLogFile(const std::filesystem::path& path)
{
    std::cout << "\nProcessing started, this might take a moment...\n\n";
    return mStorage.loadLogFile(path);
}

void LogManager::printQueredData(const LogQuery& query) const
{
    const auto context = mStorage.getLogContext();
    QueryEngine engine(context);
    auto resultIds = engine.query(query);

    for (auto id : resultIds)
        std::cout << context.entries[id] << '\n';

    std::cout << "Query has resulted in: " << resultIds.size() << " entries.\n";
}

//4823
//file=large_logs.txt level=ERROR source=PaymentGateway msg=Transaction