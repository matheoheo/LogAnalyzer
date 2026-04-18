#include "LogEntry.h"
#include <charconv>

LogEntry::TimePoint LogEntry::parseTimestamp(std::string_view s) 
{
    int y = 0;
    int m = 0;
    int d = 0;
    int hh = 0;
    int mm = 0;
    int ss = 0;

    std::from_chars(s.data(), s.data() + 4, y);
    std::from_chars(s.data() + 5, s.data() + 7, m);
    std::from_chars(s.data() + 8, s.data() + 10, d);
    std::from_chars(s.data() + 11, s.data() + 13, hh);
    std::from_chars(s.data() + 14, s.data() + 16, mm);
    std::from_chars(s.data() + 17, s.data() + 19, ss);

    std::chrono::year_month_day ymd{
        std::chrono::year{y},
        std::chrono::month{static_cast<unsigned int>(m)},
        std::chrono::day{static_cast<unsigned int>(d)}
    };

    auto time = std::chrono::hours{ hh } +
        std::chrono::minutes{ mm } +
        std::chrono::seconds{ ss };

    return std::chrono::sys_days{ ymd } + time;
}
