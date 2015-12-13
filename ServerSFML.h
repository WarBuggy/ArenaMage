#ifndef SERVERSFML_H
#define SERVERSFML_H
#include <iostream>
#include <string>
#include <sstream>
#include "Packet.h"
#include "Endpoint.h"

class ServerSFML
{
public:
    ServerSFML() {};
    ServerSFML(std::string pass, size_t serverCap, size_t teamCap, uint32_t port);
    static sf::UdpSocket socket_;
    static bool StopServer;
    void run();
    void stop();
private:
    bool IsServerOnline = false;
    void send(Packet p, Endpoint remoteEndpoint);
    void ProcessReceivedPacket(Packet p, Endpoint remoteEndpoint);
    void ProcessLogOutPacket(Packet p, Endpoint remoteEndpoint);
    void ProcessLogInPacket(Packet p, Endpoint remoteEndpoint);
    void ProcessRequestArena(Endpoint ep);
    std::string Password;
    size_t MaxServerCapacity, MaxTeamCapacity;
    std::vector<Endpoint> CurrentPlayers;
    Arena arena;
};
#endif


