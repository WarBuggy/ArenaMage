#include "ClientSFML.h"
sf::UdpSocket ClientSFML::socket_;
bool ClientSFML::StopClient;

ClientSFML::ClientSFML(std::string pass, std::string name, std::string ip, size_t server_port)
{
    State = STATE::Init;
    StopClient = false;
    logStream.clear();
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
            std::cerr << "Error in setup client socket. Client is not started." << std::endl;
        }
        else
        {
            std::cerr << "Unexpected client socket status when during setting up. Client is not started." << s << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error in setting up client socket: " << std::endl;
        std::cerr << e.what() << std::endl;
        return;
    }
}

void ClientSFML::send(Packet p)
{
    sf::Socket::Status status = socket_.send(p.p, serverIP, (uint16_t)serverPort);
    if (status == sf::Socket::Error)
    {
        std::cout << "Error in sending data to server." << std::endl;
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
    else
    {
        if (p.ID == DataID::LogInDenyWrongPassword)
        {
            out << "Wrong password. Authen failed.";
        }
        else if (p.ID == DataID::LogInDenyMaxPlayer)
        {
            out << "Server is full. Authen failed.";
        }
    }
    std::cout << out.str() << std::endl;
}

void ClientSFML::StartAuthen()
{
    Packet p;
    p.CreateAuthenPacket(Pass, Name);
    send(p);
}

void ClientSFML::run()
{
    size_t received;
    sf::IpAddress sender;
    uint16_t port;
    uint8_t data[Packet::BUFFER_SIZE];
    StartAuthen();
    while (StopClient == false)
    {
        sf::Packet receivedPacket;
        sf::Socket::Status status = socket_.receive(receivedPacket, sender, port);
        if (status == sf::Socket::Error)
        {
            // error...
            std::cout << "Error in receiving from server." << std::endl;
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
                std::cout << "Error in client handle_receive:" << std::endl;
                std::cout << e.what() << std::endl;
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
        std::cout << "Client socket unbinds." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error in client closing socket:" << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

void ClientSFML::Logout()
{
    Packet p;
    p.CreateDataIDOnly(DataID::LogOut);
    send(p);
}