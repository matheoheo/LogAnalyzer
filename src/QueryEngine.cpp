#include "QueryEngine.h"
#include <algorithm>

QueryEngine::QueryEngine(const LogContext& context)
	:mContext(context)
{
}

std::vector<size_t> QueryEngine::query(const LogQuery& query) const
{
	auto plan = getQueryPlan(query);

	if (plan.empty())
		return mContext.timestampIndexes;

	const auto& bestStep = plan.front();
	if (bestStep.size == 0)
		return {};

	std::vector<size_t> result;
	if (bestStep.type == QueryStep::Type::IndexVector)
		result = *bestStep.dataPtr;
	else if (bestStep.type == QueryStep::Type::TimestampRange)
	{
		result.reserve(bestStep.size);
		for (size_t i = bestStep.range.beingIdx; i < bestStep.range.endIdx; ++i)
			result.push_back(mContext.timestampIndexes[i]);
	}

	for (size_t i = 1; i < plan.size(); ++i)
	{
		const auto& step = plan[i];
		std::erase_if(result, [&](size_t id)
			{
				if (step.type == QueryStep::Type::IndexVector)
					return !std::binary_search(step.dataPtr->begin(), step.dataPtr->end(), id);
				
				const auto& thisTime = mContext.entries[id].timestamp;
				if ((query.startTime && thisTime < *query.startTime) || (query.endTime && thisTime > *query.endTime))
					return true;

				return false;
			});

		if (result.empty())
			break;
	}

	return result;
}

std::vector<QueryEngine::QueryStep> QueryEngine::getQueryPlan(const LogQuery& query) const
{
	std::vector<QueryStep> result;

	auto addStep = [&](const auto& map, const std::optional<std::string>& queryStr)
	{
		if (!queryStr)
			return;

		auto it = map.find(*queryStr);
		if (it != std::end(map))
			result.push_back({ .type = QueryStep::Type::IndexVector, .size = it->second.size(), .dataPtr = &it->second });
		else
			result.push_back({ .type = QueryStep::Type::IndexVector, .size = 0 });
	};

	addStep(mContext.logLevelIndexes, query.logLevel);
	addStep(mContext.sourceIndexes, query.source);
	addStep(mContext.wordIndexes, query.phrase);

	//Timestamp handle
	if (query.startTime || query.endTime)
	{
		auto start = query.startTime
			? std::ranges::lower_bound(mContext.timestampIndexes, *query.startTime, std::less<>{}, [&](size_t idx) {
					return mContext.entries[idx].timestamp;
				})
			: mContext.timestampIndexes.begin();

		auto end = query.endTime
			? std::ranges::upper_bound(start, std::ranges::end(mContext.timestampIndexes), *query.endTime, std::less<>{}, [&](size_t idx) {
					return mContext.entries[idx].timestamp;
				})
			: mContext.timestampIndexes.end();

		size_t dist = static_cast<size_t>(std::ranges::distance(start, end));
		QueryStep::Range r{
			.beingIdx = static_cast<size_t>(std::ranges::distance(std::ranges::begin(mContext.timestampIndexes), start)),
			.endIdx = static_cast<size_t>(std::ranges::distance(std::ranges::begin(mContext.timestampIndexes), end))
		};
		result.push_back({ .type = QueryStep::Type::TimestampRange, .size = dist, .dataPtr = &mContext.timestampIndexes, .range = r });
	}

	std::ranges::sort(result, std::less<>{}, & QueryStep::size);

	return result;
}
