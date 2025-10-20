#ifndef DATE_UTIL_H
#define DATE_UTIL_H

#include <ctime>
#include <string>

class DateUtil {
public:
    
    static bool isWithinLastDays(time_t timestamp, int days) {
        time_t now = time(nullptr);
        time_t cutoff = now - (days * 24 * 60 * 60);
        return timestamp >= cutoff;
    }

    static bool isToday(time_t timestamp) {
        time_t now = time(nullptr);
        tm* tm_now = localtime(&now);
        tm* tm_ts = localtime(&timestamp);
        
        return (tm_ts->tm_year == tm_now->tm_year &&
                tm_ts->tm_mon == tm_now->tm_mon &&
                tm_ts->tm_mday == tm_now->tm_mday);
    }
    
    static std::string formatTime(time_t timestamp) {
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
        return std::string(buffer);
    }
};

#endif