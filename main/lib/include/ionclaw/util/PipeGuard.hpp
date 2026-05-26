#pragma once

#include <cstdio>

#if defined(_WIN32)
#define IONCLAW_POPEN _popen
#define IONCLAW_PCLOSE _pclose
#else
#define IONCLAW_POPEN popen
#define IONCLAW_PCLOSE pclose
#endif

namespace ionclaw
{
namespace util
{

class PipeGuard
{
public:
    explicit PipeGuard(const char *command, const char *mode = "r")
        : pipe(command ? IONCLAW_POPEN(command, mode) : nullptr)
    {
    }

    ~PipeGuard()
    {
        if (pipe)
        {
            IONCLAW_PCLOSE(pipe);
        }
    }

    PipeGuard(const PipeGuard &) = delete;
    PipeGuard &operator=(const PipeGuard &) = delete;

    FILE *get() const { return pipe; }
    explicit operator bool() const { return pipe != nullptr; }

    int close()
    {
        if (!pipe)
        {
            return -1;
        }

        auto result = IONCLAW_PCLOSE(pipe);
        pipe = nullptr;
        return result;
    }

private:
    FILE *pipe;
};

} // namespace util
} // namespace ionclaw
