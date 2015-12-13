#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include<vector>
#include "DataID.h"
#include "Arena.h"
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


//    Packet();
//    Packet(std::vector<unsigned char> data, size_t size);
//    DataID GetDataID();
//    void SetDataID(DataID id);
//    std::vector<unsigned char> GetData();
//    void SetData(std::vector<unsigned char> data);
//    std::vector<unsigned char> GetDataStream();
//    void CreateAuthenPacket(std::string pass, std::string name);
//    void CreateDataIDOnly(DataID id);
//    std::string GetDataStreamAsString();
//    void AddArenaObjectsPacket(ArenaObject obj);
//private:
//    std::vector<unsigned char> _Data;
//    DataID _DataID;