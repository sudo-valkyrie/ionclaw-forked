#include "ionclaw/bus/Events.hpp"

namespace ionclaw
{
namespace bus
{

std::string InboundMessage::sessionKey() const
{
    return channel + ":" + chatId;
}

} // namespace bus
} // namespace ionclaw
