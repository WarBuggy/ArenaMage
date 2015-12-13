#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include<vector>
#include "DataID.h"
#include "SFML/Network/Packet.hpp"

class Packet
{
public:
    static const uint16_t BUFFER_SIZE = 1024;
    DataID ID;
    Packet() {};
    Packet(sf::Packet receivedPacket);
    void CreateAuthenPacket(std::string pass, std::string name);
    void CreateDataIDOnly(DataID id);
    sf::Packet p;
private:
    
};

#endif
