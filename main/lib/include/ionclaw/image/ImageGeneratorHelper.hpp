#pragma once

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace ionclaw
{
namespace image
{

class ImageGeneratorHelper
{
public:
    static std::string saveToPublicMedia(const std::string &imageData, const std::string &publicPath, const std::string &filename, const std::string &publicUrl);
    static std::string cleanReferencePath(const std::string &raw);
    static std::string extractModelId(const std::string &model);
    static std::vector<std::string> resolveReferencePaths(const nlohmann::json &params, const std::string &projectPath, const std::string &workspacePath, const std::string &publicPath, bool restrictToWorkspace);
    static std::string decodeAndSave(const std::string &responseBody, const std::string &publicPath, const std::string &filename, const std::string &publicUrl);
};

} // namespace image
} // namespace ionclaw
