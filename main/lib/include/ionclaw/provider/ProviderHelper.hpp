#pragma once

#include <string>

#include "nlohmann/json.hpp"

namespace ionclaw
{
namespace provider
{

class ProviderHelper
{
public:
    static std::string stripProviderPrefix(const std::string &model);
    static std::string sanitizeToolCallId(const std::string &id, const std::string &prefix = "call_");
    static nlohmann::json repairJsonArgs(const std::string &args);
    static std::string sanitizeErrorMessage(const std::string &msg);
    static std::string classifyError(const std::string &msg);
    static void sanitizeToolCallInputs(nlohmann::json &messages);
    static std::string sanitizeToolCallName(const std::string &name);
};

} // namespace provider
} // namespace ionclaw
