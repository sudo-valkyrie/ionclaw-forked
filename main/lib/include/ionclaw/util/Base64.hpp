#pragma once

#include <cstddef>
#include <string>

namespace ionclaw
{
namespace util
{

class Base64
{
public:
    static std::string encode(const unsigned char *data, size_t len);
    static std::string encode(const std::string &data);
    static std::string decode(const std::string &encoded);
    static std::string encodeFromFile(const std::string &path);

private:
    static const char ENCODE_CHARS[];
    static const unsigned char DECODE_TABLE[];
};

} // namespace util
} // namespace ionclaw
