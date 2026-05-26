#pragma once

#include <sstream>
#include <string>

#include "Poco/Net/PartSource.h"

#include "ionclaw/transcription/TranscriptionProvider.hpp"

namespace ionclaw
{
namespace transcription
{

class OpenAITranscriptionProvider final : public TranscriptionProvider
{
public:
    explicit OpenAITranscriptionProvider(const std::string &providerName);

    std::string providerName() const override;
    TranscriptionResult transcribe(const std::string &audioData, const std::string &format, const TranscriptionContext &context) const override;

private:
    std::string name;

    static std::string audioMimeType(const std::string &format);
    static std::string stripModelPrefix(const std::string &model);

    class StringPartSource : public Poco::Net::PartSource
    {
    public:
        StringPartSource(const std::string &data, const std::string &mediaType, const std::string &filename);
        std::istream &stream() override;
        const std::string &filename() const override;

    private:
        std::string content;
        std::istringstream contentStream;
        std::string fileName;
    };
};

} // namespace transcription
} // namespace ionclaw
