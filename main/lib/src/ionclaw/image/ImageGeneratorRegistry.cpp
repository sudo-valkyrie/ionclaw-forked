#include "ionclaw/image/ImageGeneratorRegistry.hpp"

#include "ionclaw/image/GeminiImageGenerator.hpp"
#include "ionclaw/image/GrokImageGenerator.hpp"
#include "ionclaw/image/OpenAIImageGenerator.hpp"

namespace ionclaw
{
namespace image
{

ImageGeneratorRegistry::ImageGeneratorRegistry()
{
    registerGenerator("gemini", std::make_unique<GeminiImageGenerator>("gemini"));
    registerGenerator("google", std::make_unique<GeminiImageGenerator>("google"));
    registerGenerator("openai", std::make_unique<OpenAIImageGenerator>("openai"));
    registerGenerator("grok", std::make_unique<GrokImageGenerator>("grok"));
}

ImageGeneratorRegistry &ImageGeneratorRegistry::instance()
{
    static ImageGeneratorRegistry inst;
    return inst;
}

void ImageGeneratorRegistry::registerGenerator(const std::string &name, std::unique_ptr<ImageGenerator> generator)
{
    if (generator)
    {
        generators[name] = std::move(generator);
    }
}

ImageGenerator *ImageGeneratorRegistry::get(const std::string &name) const
{
    auto it = generators.find(name);

    if (it != generators.end())
    {
        return it->second.get();
    }

    return nullptr;
}

std::vector<std::string> ImageGeneratorRegistry::providerNames() const
{
    std::vector<std::string> names;
    names.reserve(generators.size());

    for (const auto &p : generators)
    {
        names.push_back(p.first);
    }

    return names;
}

} // namespace image
} // namespace ionclaw
