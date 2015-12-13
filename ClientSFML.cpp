#include "ClientSFML.h"
sf::UdpSocket ClientSFML::socket_;
bool ClientSFML::StopClient;

ClientSFML::ClientSFML(std::string pass, std::string name, std::string ip, size_t server_port)
{
    State = STATE::Init;
    StopClient = false;
    logStream.clear();
    TotalArenaObject = 0;
    try
    {
        sf::Socket::Status s = socket_.bind(sf::Socket::AnyPort);
        if (s == sf::Socket::Done)
        {
            socket_.setBlocking(false);
            Pass = pass;
            Name = name;
            socket_.setBlocking(false);
            serverPort = server_port;
            serverIP = sf::IpAddress(ip);
        }
        else if (s == sf::Socket::Error)
        {
            Log("Error in setup client socket. Client is not started.", true);
        }
        else
        {
            Log("Unexpected client socket status when during setting up. Client is not started.", true);
        }
    }
    catch (std::exception& e)
    {
        Log("Error in setting up client socket: ", true);
        Log(e.what(), true);
        return;
    }
}

void ClientSFML::send(Packet p)
{
    sf::Socket::Status status = socket_.send(p.p, serverIP, (uint16_t)serverPort);
    if (status == sf::Socket::Error)
    {
        Log("Error in sending data to server.", true);
    }
    else if (status == sf::Socket::Done)
    {
        // done
    }
}

void ClientSFML::ProcessReceivedPacket(Packet p)
{
    std::stringstream out;
    if (p.ID == DataID::LogInServerAccept)
    {
        out << "Authen complete";
        State = STATE::AuthenCompleted;
        Packet p;
        p.CreateDataIDOnly(DataID::RequestArenaInfo);
        send(p);
    }
    else if (p.ID == DataID::LogInDenyWrongPassword)
    {
        out << "Wrong password. Authen failed.";
    }
    else if (p.ID == DataID::LogInDenyMaxPlayer)
    {
        out << "Server is full. Authen failed.";
    }
    else if (p.ID == DataID::ArenaObjects)
    {
        ProcessArenaPackages(p);
    }
    else if (p.ID == DataID::ArenaInfo)
    {
        ProcessArenaInfoPackage(p);
    }
    Log(out.str());
}

void ClientSFML::StartAuthen()
{
    Packet p;
    p.CreateAuthenPacket(Pass, Name);
    send(p);
}

void ClientSFML::run()
{
    sf::IpAddress sender;
    uint16_t port;
    StartAuthen();
    while (StopClient == false)
    {
        sf::Packet receivedPacket;
        sf::Socket::Status status = socket_.receive(receivedPacket, sender, port);
        if (status == sf::Socket::Error)
        {
            // error...
            Log("Error in receiving from server.", true);
        }
        else if (status == sf::Socket::Done)
        {
            try
            {
                Packet p = Packet(receivedPacket);
                ProcessReceivedPacket(p);
                //std::cout << "Size received: " << size << std::endl;
                //std::cout << p.GetDataStreamAsString() << std::endl;
            }
            catch (std::exception& e)
            {
                Log("Error in client handle_receive:", true);
                Log(e.what(), true);
            }
        }
        else
        {
            //std::cout << "Socket status: " << status << std::endl;
        }
    }
    // exit loop means client is stopped
    stop();
}

void ClientSFML::stop()
{
    try
    {
        if (State >= STATE::AuthenCompleted)
        {
            Logout();
        }
        socket_.unbind();
        Log("Client socket unbinds.");
    }
    catch (std::exception& e)
    {
        Log("Error in client closing socket: ", true);
        Log(e.what(), true);
    }
}

void ClientSFML::Logout()
{
    Packet p;
    p.CreateDataIDOnly(DataID::LogOut);
    send(p);
}

void ClientSFML::ProcessArenaPackages(Packet p)
{
    if (TotalArenaObject == 0)
    {
        // client hasnt received arena info yet
        // re-request arenainfo
        // might cause some kind of dead-lock here
        //sf::Packet p;
        //p << (uint8_t)DataID::RequestArenaInfo;
        //send(p);
        return;
    }
    uint8_t packageNum = 0, totalPackageNum = 0, objectNum = 0;
    bool success = p.p >> packageNum >> totalPackageNum >> objectNum;
    if (!success)
    {
        // error
        Log("Error processing arena package header");
    }
    else
    {
        // check if packageNum exists
        size_t count = packageReceived.count(packageNum);
        if (count == 1)
        {
            // this pack is duplicated, do nothing
            std::stringstream out;
            out << "Duplicated arena package #" << packageNum << " received.";
            Log(out.str());
        }
        else
        {
            std::stringstream out;
            packageReceived.insert(std::pair<uint8_t, bool>(packageNum, true));
            for (uint8_t i = 0; i <= objectNum - 1; i++)
            {
                Point2D point;
                uint8_t type;
                success = p.p >> point.X >> point.Y >> type;
                if (!success)
                {
                    out << "Error processing arena package object #" << (i + packageNum*Arena::OBJECT_SENT_PER_PACKAGE);
                    Log(out.str());
                }
                else
                {
                    arena.ArenaObjects.insert(std::pair<Point2D, ObjectType>(point, (ObjectType)type));
                    if (arena.ArenaObjects.size() == TotalArenaObject)
                    {
                        //sf::Packet reply;
                        //reply << (uint8_t)DataID::ArenaReceivedCompleted;
                        //send(reply);
                        // Im an asshole and i dont send reply and shit
                        Log("All arena objects were received succesfully.");
                        State = STATE::ArenaInfoCompleted;
                        return;
                    }
                }
            }
        }
    }
}

void ClientSFML::ProcessArenaInfoPackage(Packet p)
{
    sf::Uint16 width = 0, height = 0, jumpHeight = 0, totalObject = 0;
    bool success = p.p >> width >> height >> jumpHeight >> totalObject;
    if (!success)
    {
        // error
        Log("Error processing arena info package from server");
    }
    else
    {
        arena = Arena(width, height, jumpHeight);
        TotalArenaObject = totalObject;
        arena.ArenaObjects = std::map<Point2D, ObjectType>();
        packageReceived = std::map<uint8_t, bool>();
    }
}

void ClientSFML::Log(std::string message, bool isError)
{
    if (!isError)
    {
        std::cout << message << std::endl;
    }
    else
    {
        std::cerr << message << std::endl;
    }
}
