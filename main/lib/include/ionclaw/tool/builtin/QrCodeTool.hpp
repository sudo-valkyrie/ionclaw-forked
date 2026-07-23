#pragma once

#include "ionclaw/tool/Tool.hpp"

namespace ionclaw::tool::builtin {

/// Generate QR code PNG images from text data.
/// Uses libqrencode for QR encoding and stb_image_write for PNG output.
class QrCodeTool final : public Tool
{
public:
    ToolResult execute(const nlohmann::json &params, const ToolContext &context) override;
    ToolSchema schema() const override;

private:
    /// Write a QRcode struct to a PNG file.
    /// Returns the output file path on success, or an error string.
    static std::string writeQrPng(const unsigned char *data, int width,
                                  const std::string &outputPath,
                                  int scale, int border);
};

} // namespace ionclaw::tool::builtin
