#pragma once

#include <optional>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace ionclaw
{
namespace bus
{

enum class QueueMode
{
    Steer,
    Followup,
    Collect,
    SteerBacklog,
    Interrupt,
};

enum class QueueDropPolicy
{
    Old,
    New,
    Summarize,
};

struct InboundMessage
{
    std::string channel;
    std::string senderId;
    std::string chatId;
    std::string content;
    std::vector<std::string> media;
    nlohmann::json metadata;
    std::optional<QueueMode> queueMode;

    std::string sessionKey() const;
};

struct OutboundMessage
{
    std::string channel;
    std::string chatId;
    std::string content;
    nlohmann::json metadata;
};

} // namespace bus
} // namespace ionclaw
