#include "Packet.h"

Packet::Packet(sf::Packet receivedPacket)
{
    p = receivedPacket;
    uint8_t id;
    p >> id;
    ID = (DataID)id;
}

void Packet::CreateAuthenPacket(std::string pass, std::string name)
{
    p << (sf::Uint8)DataID::LogInRequestClient;
    // authen packet format
    // Password length + password + name length + name
    p << (uint8_t)pass.size();
    p << pass;
    p << (uint8_t)name.size();
    p << name;
}

void Packet::CreateDataIDOnly(DataID id)
{
    ID = id;
    p << (sf::Uint8)id;
}
