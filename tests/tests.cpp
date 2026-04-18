#include <gtest/gtest.h>
#include "LogStorage.h"
#include "QueryEngine.h"

TEST(LogSystemTest, ParsesCorrectly)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [ERROR] [Auth] Connection failed");

	EXPECT_EQ(storage.getLogContext().entries.size(), 1);
}

TEST(LogSystemTest, FilterByLevel)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [S1] Ok");
	storage.addLogEntry("[2023-10-25T10:01:00] [ERROR] [S1] Error");
	auto ctx = storage.getLogContext();

	LogQuery q;
	q.logLevel = "ERROR";
	QueryEngine engine(ctx);
	auto filtered = engine.query(q);

	EXPECT_EQ(filtered.size(), 1);
}

TEST(LogSystemTest, FilterBySource)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [S1] Ok");
	storage.addLogEntry("[2023-10-25T10:01:00] [ERROR] [S2] Error");
	auto ctx = storage.getLogContext();

	LogQuery q;
	q.source = "S1";
	QueryEngine engine(ctx);
	auto filtered = engine.query(q);

	EXPECT_EQ(filtered.size(), 1);
}

TEST(LogSystemTest, FilterByMessage)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [S1] Payment rejected.");
	storage.addLogEntry("[2023-10-25T10:01:00] [ERROR] [S2] Payment accepted.");
	auto ctx = storage.getLogContext();

	LogQuery q;
	q.phrase = "accepted";
	QueryEngine engine(ctx);
	auto filtered = engine.query(q);

	EXPECT_EQ(filtered.size(), 1);
}

TEST(LogSystemTest, FilterByTime)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [S1] Payment rejected.");
	storage.addLogEntry("[2023-10-25T10:01:00] [ERROR] [S2] Payment accepted.");
	storage.addLogEntry("[2023-10-25T10:02:00] [DEBUG] [S3] Payment accepted.");
	storage.addLogEntry("[2023-10-25T10:03:00] [SUCCESS] [S4] Payment accepted.");

	auto ctx = storage.getLogContext();

	LogQuery q;
	q.startTime = LogEntry::parseTimestamp("2023-10-25T10:01:00");
	QueryEngine engine(ctx);
	auto filtered = engine.query(q);
	EXPECT_EQ(filtered.size(), 3);

	q.endTime = LogEntry::parseTimestamp("2023-10-25T10:02:00");
	filtered = engine.query(q);
	EXPECT_EQ(filtered.size(), 2);
}

TEST(LogSystemTest, MultipleFilters)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [AuthService] User logged in successfully");
	storage.addLogEntry("[2023-10-25T10:05:12] [ERROR] [Database] Connection timeout");
	storage.addLogEntry("[2023-10-25T10:15:30] [WARN] [AuthService] Multiple failed login attempts");
	storage.addLogEntry("[2023-10-25T10:20:00] [ERROR] [Payment] Transaction rejected: insufficient funds");
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [AuthService] User logged in successfully");
	storage.addLogEntry("[2023-10-25T10:25:00] [INFO] [Payment] Transaction 12345 processed");

	auto ctx = storage.getLogContext();

	QueryEngine engine(ctx);
	LogQuery q;
	q.levelEquals("ERROR");
	q.timeBetween(LogEntry::parseTimestamp("2023-10-25T10:00:00"), LogEntry::parseTimestamp("2023-10-25T10:10:00"));
	
	auto filtered = engine.query(q);
	EXPECT_EQ(filtered.size(), 1);
}

TEST(LogSystemTest, NoResultsFound)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [S1] Test");
	auto ctx = storage.getLogContext();

	LogQuery q;
	q.logLevel = "CRITICAL_ERROR_THAT_DOES_NOT_EXIST";
	QueryEngine engine(ctx);
	auto filtered = engine.query(q);

	EXPECT_TRUE(filtered.empty());
}

TEST(LogSystemTest, CaseSensitivityLevel)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [ERROR] [S1] Msg");
	auto ctx = storage.getLogContext();

	LogQuery q;
	q.logLevel = "error"; 
	QueryEngine engine(ctx);

	EXPECT_EQ(engine.query(q).size(), 0);
}

TEST(LogSystemTest, ReversedTimeRange)
{
	LogStorage storage;
	storage.addLogEntry("[2023-10-25T10:00:00] [INFO] [S1] Msg");
	auto ctx = storage.getLogContext();

	LogQuery q;
	q.startTime = LogEntry::parseTimestamp("2024-01-01T00:00:00");
	q.endTime = LogEntry::parseTimestamp("2023-01-01T00:00:00");

	QueryEngine engine(ctx);
	EXPECT_TRUE(engine.query(q).empty());
}