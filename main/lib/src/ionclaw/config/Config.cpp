#include "ionclaw/config/Config.hpp"

#include <cctype>
#include <stdexcept>

namespace ionclaw
{
namespace config
{

std::string Config::resolveApiKey(const std::string &providerName) const
{
    auto provIt = providers.find(providerName);

    if (provIt == providers.end())
    {
        return "";
    }

    const auto &provider = provIt->second;
    auto credIt = credentials.find(provider.credential);

    if (credIt == credentials.end())
    {
        return "";
    }

    // prefer key, fall back to token
    std::string key = credIt->second.key;

    if (!key.empty())
    {
        return key;
    }

    return credIt->second.token;
}

std::string Config::resolveBaseUrl(const std::string &providerName) const
{
    auto provIt = providers.find(providerName);

    if (provIt != providers.end() && !provIt->second.baseUrl.empty())
    {
        return provIt->second.baseUrl;
    }

    // defaults for known providers
    if (providerName == "anthropic")
    {
        return "https://api.anthropic.com";
    }

    if (providerName == "openai")
    {
        return "https://api.openai.com";
    }

    if (providerName == "google" || providerName == "gemini")
    {
        return "https://generativelanguage.googleapis.com/v1beta/openai";
    }

    return "";
}

ProviderConfig Config::resolveProvider(const std::string &model) const
{
    // extract prefix from model string (e.g., "anthropic/claude-3-5-sonnet" -> "anthropic")
    auto slashPos = model.find('/');
    std::string prefix;
    std::string suffix;

    if (slashPos != std::string::npos)
    {
        prefix = model.substr(0, slashPos);
        suffix = model.substr(slashPos + 1);
    }
    else
    {
        prefix = model;
    }

    // the "llama/" prefix loads a local model file
    // find the provider whose base_url points to a .gguf file matching the suffix
    if (prefix == "llama")
    {
        for (const auto &[key, provider] : providers)
        {
            if (provider.baseUrl.size() < 5) continue;
            // check if base_url ends with .gguf
            if (provider.baseUrl.compare(provider.baseUrl.size() - 5, 5, ".gguf") != 0)
            {
                continue;
            }
            // get file name from path
            auto lastSlash = provider.baseUrl.find_last_of("/\\");
            auto fileName = (lastSlash != std::string::npos) ? provider.baseUrl.substr(lastSlash + 1) : provider.baseUrl;
            // no suffix or suffix matches start of file name (case-insensitive)
            if (suffix.empty())
            {
                return provider;
            }
            if (fileName.size() >= suffix.size())
            {
                auto filePrefix = fileName.substr(0, suffix.size());
                bool match = true;
                for (size_t i = 0; i < suffix.size(); ++i)
                {
                    if (std::tolower(filePrefix[i]) != std::tolower(suffix[i]))
                    {
                        match = false;
                        break;
                    }
                }
                if (match)
                {
                    return provider;
                }
            }
        }
        throw std::runtime_error("[Config] No llama provider found for model: " + model);
    }

    // find provider matching the prefix
    auto provIt = providers.find(prefix);

    if (provIt != providers.end())
    {
        return provIt->second;
    }

    // search all providers for a name match
    for (const auto &[key, provider] : providers)
    {
        if (provider.name == prefix)
        {
            return provider;
        }
    }

    throw std::runtime_error("[Config] No provider found for model: " + model);
}

} // namespace config
} // namespace ionclaw
