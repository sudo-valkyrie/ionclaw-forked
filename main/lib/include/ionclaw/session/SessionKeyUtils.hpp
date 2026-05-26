#pragma once

#include <optional>
#include <string>

namespace ionclaw
{
namespace session
{

struct ParsedSessionKey
{
    std::string agentId;
    std::string channel;
    std::string chatId;
    std::string baseKey;
};

class SessionKeyUtils
{
public:
    static std::string build(const std::string &agentId, const std::string &channel, const std::string &chatId);
    static std::string buildFromBase(const std::string &agentId, const std::string &baseKey);
    static std::optional<ParsedSessionKey> parse(const std::string &key);
    static std::string extractChannel(const std::string &key);
    static std::string extractBaseKey(const std::string &key);
    static std::string extractChatId(const std::string &key);
    static bool isAgentScoped(const std::string &key);

    static constexpr const char *AGENT_PREFIX = "agent:";
};

} // namespace session
} // namespace ionclaw
