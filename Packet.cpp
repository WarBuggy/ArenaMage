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
    p << (uint8_t)DataID::LogInRequestClient;
    // authen packet format
    // Password length + password + name length + name
    p << (uint8_t)pass.size();
    p << pass;
    p << (uint8_t)name.size();
    p << name;
}

void Packet::CreateDataIDOnly(DataID id)
{
    p << id;
}

/*
Packet::Packet()
{
    _DataID = DataID::Null;
    _Data = std::vector<unsigned char>();
}

Packet::Packet(std::vector<unsigned char> data, size_t receivedSize)
{
    size_t size = data.size();
    if (size <= 0)
    {
        _DataID = DataID::Null;
        _Data = std::vector<unsigned char>();
    }
    else
    {
        // get message header
        _DataID = (DataID)(data[0]);
        // get message length
        if (size > 2)
        {
            uint16_t length = data[2] << 8 | data[1];

            if (size >= (size_t)(length + 3) && receivedSize == length + 3) // 3 for 1 header byte and 2 length byte
            {
                _Data = std::vector<unsigned char>(length);
                std::copy(data.begin() + 3, data.begin() + 3 + length, _Data.begin());
            }
        }
        else
        {
            _Data = std::vector<unsigned char>();
        }
    }
}

DataID Packet::GetDataID()
{
    return _DataID;
}

void Packet::SetDataID(DataID id)
{
    _DataID = id;
}

std::vector<unsigned char> Packet::GetData()
{
    return _Data;
}

void Packet::SetData(std::vector<unsigned char> data)
{
    _Data = data;
}

std::vector<unsigned char> Packet::GetDataStream()
{
    uint16_t dataSize = (uint16_t)_Data.size();
    if (dataSize == 0)
    {
        std::vector<unsigned char> dataStream = std::vector<unsigned char>(1);
        dataStream[0] = (unsigned char)_DataID;
        return dataStream;
    }
    else
    {
        std::vector<unsigned char> dataStream = std::vector<unsigned char>(3 + dataSize);
        dataStream[0] = (unsigned char)_DataID;
        // Add message length
        // taken from http://stackoverflow.com/questions/13279024/convert-a-uint16-t-to-char2-to-be-sent-over-socket-unix
        unsigned char low = dataSize & 0xFF;
        unsigned char hi = dataSize >> 8;
        dataStream[1] = low;
        dataStream[2] = hi;
        // Add message
        std::copy(_Data.begin(), _Data.end(), dataStream.begin() + 3);
        return dataStream;
    }
}

void Packet::CreateAuthenPacket(std::string pass, std::string name)
{
    _DataID = DataID::LogInRequestClient;
    // authen packet format
    // Password length + password + name length + name
    _Data = std::vector<unsigned char>(1 + pass.size() + 1 + name.size());
    _Data[0] = (unsigned char)pass.size();
    if (pass.size() > 0)
    {
        std::copy(pass.begin(), pass.end(), _Data.begin() + 1);
    }
    _Data[1 + pass.size()] = (unsigned char)name.size();
    std::copy(name.begin(), name.end(), _Data.begin() + 1 + pass.size() + 1);
}

void Packet::CreateDataIDOnly(DataID id)
{
    _DataID = id;
}

std::string Packet::GetDataStreamAsString()
{
    std::vector<unsigned char> stream = GetDataStream();
    return std::string(stream.begin(), stream.end());
}

void Packet::AddArenaObjectsPacket(ArenaObject obj)
{
}
*/

