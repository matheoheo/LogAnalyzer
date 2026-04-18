#pragma once
#include "LogContext.h"
#include "LogQuery.h"

class QueryEngine
{
public:
	QueryEngine(const LogContext& context);
	std::vector<size_t> query(const LogQuery& query) const;
private:
	struct QueryStep
	{
		enum class Type {IndexVector, TimestampRange};
		Type type;
		size_t size = 0;
		const std::vector<size_t>* dataPtr = nullptr;

		//Range for TimestampRange
		struct Range { size_t beingIdx = 0; size_t endIdx = 0; };
		Range range;
	};

	std::vector<QueryStep> getQueryPlan(const LogQuery& query) const;
private:
	const LogContext mContext;
};