#include "LogManager.h"
#include "LogInterface.h"
#include <iostream>

void printCommands();
void handleInput(std::filesystem::path& p, LogQuery& q);

int main(int argc, char* argv[])
{
    std::filesystem::path p;
    LogQuery q;

    std::cout << "==========================================================\n";
    std::cout << "                       LOG ANALYZER                       \n";
    std::cout << "==========================================================\n\n";

    if (argc > 1)
    {
        auto result = LogInterface::parseArgs(argc, argv);
        p = result.pathToFile;
        q = result.q;
    }
    else
    {
        printCommands();
        handleInput(p, q);
    }
    //"large_logs.txt";
    if (p.empty())
    {
        std::cout << "Failed to provide file path, cannot continue\n";
        return 0;
    }

    LogManager manager;
    if (!manager.loadLogFile(p))
        return 0;
    while (true)
    {
        manager.printQueredData(q);
        std::cout << "----------------------------------------------------------\n";
        std::cout << "Enter next filters or type 'exit' to quit:\n> ";

        std::string input;
        if (!std::getline(std::cin, input) || input == "exit") 
            break;

        auto result = LogInterface::parseLine(input);

        if (!result.pathToFile.empty() && result.pathToFile != p)
        {
            p = result.pathToFile;
            if (!manager.loadLogFile(p))
                return 0;
        }

        q = result.q;
    }

	return 0;
}

void printCommands()
{
    std::cout << "Usage: Enter commands as key=value pairs separated by spaces.\n";
    std::cout << "Available keys:\n";
    std::cout << "  file=<path>      (Required) Path to the log file\n";
    std::cout << "  level=<value>    Filter by log level (e.g., INFO, ERROR)\n";
    std::cout << "  source=<value>   Filter by source component\n";
    std::cout << "  msg=<value>      Search for a phrase in the message\n";
    std::cout << "  start=<iso>      Start time (format: YYYY-MM-DDTHH:MM:SS)\n";
    std::cout << "  end=<iso>        End time (format: YYYY-MM-DDTHH:MM:SS)\n\n";
    std::cout << "Example:\n";
    std::cout << "  file=logs.txt source=Database level=ERROR msg=Transaction\n";
    std::cout << "----------------------------------------------------------\n";
    std::cout << "Enter command: ";
}

void handleInput(std::filesystem::path& p, LogQuery& q)
{
    std::string input;
    std::getline(std::cin, input);
    auto result = LogInterface::parseLine(input);
    p = result.pathToFile;
    q = result.q;
}