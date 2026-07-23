#include "ionclaw/tool/builtin/QrCodeTool.hpp"

#include <chrono>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

#include "qrencode.h"
#include "stb_image_write.h"

namespace fs = std::filesystem;

namespace ionclaw::tool::builtin {

// ---------------------------------------------------------------------------
// Error-correction level mapping
// ---------------------------------------------------------------------------
static QRecLevel parseEcl(const std::string &level)
{
    if (level == "L") return QR_ECLEVEL_L;
    if (level == "M") return QR_ECLEVEL_M;
    if (level == "Q") return QR_ECLEVEL_Q;
    if (level == "H") return QR_ECLEVEL_H;
    return QR_ECLEVEL_M; // default
}

// ---------------------------------------------------------------------------
// Write a QRcode pixel buffer to a PNG file via stb_image_write.
// Each pixel is 1 byte: 0 = black, non-zero = white (libqrencode convention).
// We scale the module grid by `scale` and add `border` empty modules.
// ---------------------------------------------------------------------------
std::string QrCodeTool::writeQrPng(const unsigned char *data, int width,
                                   const std::string &outputPath,
                                   int scale, int border)
{
    // Final image dimensions
    int imgW = (width + 2 * border) * scale;
    int imgH = imgW;

    // Allocate RGBA pixel buffer (white background)
    std::vector<unsigned char> image(static_cast<size_t>(imgW) * imgH * 4, 255);

    // Draw QR modules (black = 0,0,0,255)
    for (int y = 0; y < width; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (data[y * width + x] & 0x01) // black module
            {
                int px = (x + border) * scale;
                int py = (y + border) * scale;

                for (int dy = 0; dy < scale; ++dy)
                {
                    for (int dx = 0; dx < scale; ++dx)
                    {
                        int idx = ((py + dy) * imgW + (px + dx)) * 4;
                        image[idx + 0] = 0;     // R
                        image[idx + 1] = 0;     // G
                        image[idx + 2] = 0;     // B
                        image[idx + 3] = 255;   // A
                    }
                }
            }
        }
    }

    // Write PNG
    if (!stbi_write_png(outputPath.c_str(), imgW, imgH, 4,
                        image.data(), imgW * 4))
    {
        return "Error: Failed to write PNG file: " + outputPath;
    }

    return outputPath;
}

// ---------------------------------------------------------------------------
// execute
// ---------------------------------------------------------------------------
ToolResult QrCodeTool::execute(const nlohmann::json &params,
                               const ToolContext &context)
{
    // -- required: data to encode ------------------------------------------
    if (!params.contains("data") || !params["data"].is_string())
    {
        return "Error: 'data' (string) is required — the text/URL to encode.";
    }
    std::string data = params["data"].get<std::string>();

    if (data.empty())
    {
        return "Error: 'data' must not be empty.";
    }

    // -- optional parameters -----------------------------------------------
    std::string filename = params.value("filename", "");
    std::string eclStr   = params.value("error_level", "M");
    int boxSize           = params.value("box_size", 10);
    int border            = params.value("border", 4);

    // Clamp sensible ranges
    if (boxSize < 1)   boxSize = 1;
    if (boxSize > 100) boxSize = 100;
    if (border < 0)    border = 0;
    if (border > 100)  border = 100;

    QRecLevel ecl = parseEcl(eclStr);

    // -- derive output path ------------------------------------------------
    if (filename.empty())
    {
        // Generate a unique name based on content hash (simple approach:
        // use first 8 hex chars of a running counter or timestamp-ish).
        // For simplicity we just use "qrcode_<timestamp>.png"
        auto now = std::chrono::system_clock::now();
        auto ts  = std::chrono::duration_cast<std::chrono::seconds>(
                       now.time_since_epoch()).count();
        filename = "qrcode_" + std::to_string(ts) + ".png";
    }

    // Make sure the filename has .png extension
    if (filename.size() < 4 ||
        filename.substr(filename.size() - 4) != ".png")
    {
        filename += ".png";
    }

    fs::path outputPath = fs::path(context.workspacePath) / filename;

    // -- encode ------------------------------------------------------------
    QRcode *qr = QRcode_encodeString(data.c_str(), 0, ecl, QR_MODE_8, 1);
    if (!qr)
    {
        return "Error: Failed to encode QR code. The data may be too large "
               "or contain unsupported characters.";
    }

    // -- write PNG ---------------------------------------------------------
    std::string result = writeQrPng(qr->data, qr->width,
                                    outputPath.string(), boxSize, border);

    QRcode_free(qr);

    if (result == outputPath.string())
    {
        std::ostringstream oss;
        oss << "QR code generated successfully.\n"
            << "File: " << outputPath.string() << "\n"
            << "Size: " << (boxSize * (qr->width + 2 * border)) << "x"
            << (boxSize * (qr->width + 2 * border)) << " px\n"
            << "Content: " << data;
        return oss.str();
    }

    return result;
}

// ---------------------------------------------------------------------------
// schema
// ---------------------------------------------------------------------------
ToolSchema QrCodeTool::schema() const
{
    // clang-format off
    return {
        "qrcode",
        "Generate a QR code PNG image from text, URL, or structured data "
        "(WiFi config, vCard, etc.). The image is saved to the workspace "
        "and the path is returned.",
        {
            {"type", "object"},
            {"properties",
             {
                 {"data",
                  {
                      {"type", "string"},
                      {"description",
                       "Content to encode — a URL, plain text, WiFi config "
                       "(WIFI:T:WPA;S:ssid;P:pwd;;), or vCard "
                       "(BEGIN:VCARD\\nFN:name\\nTEL:num\\nEND:VCARD)"}
                  }
                 },
                 {"filename",
                  {
                      {"type", "string"},
                      {"description",
                       "Output PNG filename (auto-generated if omitted)"}
                  }
                 },
                 {"error_level",
                  {
                      {"type", "string"},
                      {"enum", {"L", "M", "Q", "H"}},
                      {"description",
                       "Error correction: L=7%, M=15%, Q=25%, H=30%"}
                  }
                 },
                 {"box_size",
                  {
                      {"type", "integer"},
                      {"description",
                       "Size of each QR module in pixels (1-100, default 10)"}
                  }
                 },
                 {"border",
                  {
                      {"type", "integer"},
                      {"description",
                       "White border width in modules (0-100, default 4)"}
                  }
                 }
             }},
            {"required", nlohmann::json::array({"data"})}
        }
    };
    // clang-format on
}

} // namespace ionclaw::tool::builtin
