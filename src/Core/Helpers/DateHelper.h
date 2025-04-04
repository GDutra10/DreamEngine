#ifndef CORE_HELPERS_DATE_HELPER_H
#define CORE_HELPERS_DATE_HELPER_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DreamEngine::Core::Helpers
{
class DateHelper
{
   public:
    static std::string ToString(const std::chrono::time_point<std::chrono::system_clock> timePoint)
    {
        std::tm time;
        const long long timeT = std::chrono::system_clock::to_time_t(timePoint);

        if (const errno_t error = localtime_s(&time, &timeT))
        {
            return "error: " + error;
        }

        // Extract milliseconds
        const auto duration = timePoint.time_since_epoch();
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) - seconds;

        std::stringstream ss;
        ss << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
        // Add milliseconds
        ss << '.' << std::setw(3) << std::setfill('0') << milliseconds.count();

        return ss.str();
    }
};
}  // namespace DreamEngine::Core::Loggers
#endif