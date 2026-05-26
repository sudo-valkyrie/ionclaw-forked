#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

namespace ionclaw
{
namespace cron
{

class CronParser
{
public:
    static int64_t nextRun(const std::string &expr, const std::string &tz = "");
    static bool isValidTimezone(const std::string &tz);
    static bool isValidExpression(const std::string &expr);

private:
    class TzGuard
    {
    public:
        TzGuard(const std::string &tz, std::mutex &mtx);
        ~TzGuard();

        TzGuard(const TzGuard &) = delete;
        TzGuard &operator=(const TzGuard &) = delete;

        bool isOverridden() const { return overridden; }

    private:
        std::unique_lock<std::mutex> lock;
        std::string savedTz;
        bool overridden;
    };

    static int safeStoi(const std::string &s, int fallback);
    static std::vector<int> expandField(const std::string &field, int min, int max);
    static bool matchesField(int value, const std::vector<int> &allowed);

    static std::mutex tzMutex;
};

} // namespace cron
} // namespace ionclaw
