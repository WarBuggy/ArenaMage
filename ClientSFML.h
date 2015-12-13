#ifndef CLIENTSFML_H
#define CLIENTSFML_H
#include <iostream>
#include <sstream>
#include "Packet.h"
#include "Endpoint.h"
#include "Arena.h"

class ClientSFML
{
public:
    ClientSFML() {};
    ClientSFML(std::string pass, std::string name, std::string ip, size_t server_port);
    void run();
    static bool StopClient;
    std::stringstream logStream;
    enum STATE : uint8_t
    {
        Init = 0,
        AuthenCompleted = 10,
        ArenaInfoCompleted = 11
    };
    STATE State;
    Arena arena;
private:
    void send(Packet p);
    void ProcessReceivedPacket(Packet p);
    void StartAuthen();
    void stop();
    static sf::UdpSocket socket_;
    std::string Pass, Name;
    sf::IpAddress serverIP;
    size_t serverPort;
    void Logout();
    void ProcessArenaPackages(Packet p);
    void ProcessArenaInfoPackage(Packet p);
    size_t TotalArenaObject;
    std::map<uint8_t, bool> packageReceived;
    void Log(std::string message, bool isError = false);
};
#endif
