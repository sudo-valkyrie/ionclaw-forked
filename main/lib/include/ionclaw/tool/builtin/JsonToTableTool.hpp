#pragma once
#include "ionclaw/tool/Tool.hpp"

namespace ionclaw::tool::builtin {
class JsonToTableTool final : public Tool 
{
public:
    ToolResult execute(const nlohmann::json &params, const ToolContext &context) override;
    ToolSchema schema() const override;
};
} // namespace ionclaw::tool::builtin
