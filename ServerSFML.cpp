#include "ServerSFML.h"
sf::UdpSocket ServerSFML::socket_;
bool ServerSFML::StopServer;

ServerSFML::ServerSFML(std::string pass, size_t serverCap, size_t teamCap, uint32_t port)
{
    IsServerOnline = false;
    sf::Socket::Status s = socket_.bind(port);

    if (s == sf::Socket::Done)
    {
        IsServerOnline = true;
        socket_.setBlocking(false);
        StopServer = false;
        Password.assign(pass);
        MaxServerCapacity = serverCap;
        MaxTeamCapacity = teamCap;
        arena.SetWidth(160);
        arena.SetHeight(80);
        arena.SetJumpHeight(20);
        arena.SetupArenaObjects();
    }
    else if (s == sf::Socket::Error)
    {
        std::cout << "Error in setup server socket. Server is not started." << std::endl;
    }
    else
    {
        std::cout << "Unexpected server socket status when during setting up. Server is not started." << s << std::endl;
    }
}

void ServerSFML::run()
{
    if (!IsServerOnline)
    {
        return;
    }
    sf::IpAddress sender;
    uint16_t port;
    std::cout << "Server listens for request..." << std::endl;
    while (StopServer == false)
    {
        sf::Packet receivedPacket;
        sf::Socket::Status status = socket_.receive(receivedPacket, sender, port);
        if (status == sf::Socket::Error)
        {
            // error...
            std::cout << "Error in receiving from client." << std::endl;
        }
        else if (status == sf::Socket::Done)
        {
            try
            {
                Packet p = Packet(receivedPacket);
                Endpoint ep = { sender, port };
                ProcessReceivedPacket(p, ep);
                //std::cout << "Size received: " << size << std::endl;
                //std::cout << p.GetDataStreamAsString() << std::endl;
            }
            catch (std::exception& e)
            {
                std::cout << "Error in server handle_receive:" << std::endl;
                std::cout << e.what() << std::endl;
            }
        }
        else
        {
            //std::cout << "Socket status: " << status << std::endl;
        }
    }
    // exit loop means server is stopped
    stop();
}

void ServerSFML::stop()
{
    try
    {
        socket_.unbind();
        std::cout << "Server socket unbinds." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error in server closing socket:" << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

void ServerSFML::send(Packet p, Endpoint ep)
{
    try
    {
        sf::Socket::Status status = socket_.send(p.p, ep.IPAddress, (uint16_t)ep.Port); // &data[0] "convert" vector to array
        if (status == sf::Socket::Status::Done)
        {
            // data sent successfully
        }
        else if (status == sf::Socket::Done)
        {
            // error
        }
        else
        {
            // something else
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error in server sending data to client " << ep.IPAddress.toString() << "," << ep.Port << ":";
        std::cerr << e.what() << std::endl;
    }
}

void ServerSFML::ProcessReceivedPacket(Packet p, Endpoint ep)
{
    DataID id = p.ID;
    if (id == DataID::LogInRequestClient)
    {
        ProcessLogInPacket(p, ep);
    }
    else if (id == DataID::LogOut)
    {
        ProcessLogOutPacket(p, ep);
    }
    else if (id == DataID::RequestArenaInfo)
    {
        ProcessRequestArena(ep);
    }
}

void ServerSFML::ProcessLogOutPacket(Packet p, Endpoint remoteEndpoint)
{
    for (std::vector<Endpoint>::iterator it = CurrentPlayers.begin(); it != CurrentPlayers.end(); ++it)
    {
        if ((*it) == remoteEndpoint)
        {
            CurrentPlayers.erase(it);
            std::cout << "A player logged out. ";
            std::cout << "Number of available spot(s): " << (MaxServerCapacity - CurrentPlayers.size()) << "." << std::endl;
            return;
        }
    }
}

void ServerSFML::ProcessLogInPacket(Packet p, Endpoint remoteEndpoint)
{
    Packet reply = Packet();
    std::stringstream out;
    std::string pass, name;
    uint8_t passwordLength, nameLength;
    if (!(p.p >> passwordLength >> pass >> nameLength >> name))
    {
        out << "Client " << remoteEndpoint.IPAddress.toString() << ", " << remoteEndpoint.Port << " sent wrong packet format. Connection denied.";
        reply.CreateDataIDOnly(DataID::LogInDenyWrongPacketFormat);
    }
    else
    {
        if (Password.compare(pass) == 0)
        {
            out << "Client " << remoteEndpoint.IPAddress.toString() << ", " << remoteEndpoint.Port << " is authenticated.";

            if (CurrentPlayers.size() < MaxServerCapacity)
            {
                CurrentPlayers.insert(CurrentPlayers.begin(), remoteEndpoint);
                reply.CreateDataIDOnly(DataID::LogInServerAccept);

                //size_t nameLength = data[1 + passwordLength];
                //if (nameLength > 0)
                //{
                //    name.assign(data.begin() + 1 + passwordLength + 1, data.begin() + 1 + passwordLength + 1 + nameLength);
                //}
                out << " " << name << " took an empty spot.";
            }
            else
            {
                reply.CreateDataIDOnly(DataID::LogInDenyMaxPlayer);
                out << " However, no more spot left. Connection closed.";
            }
        }
        else
        {
            // incorrect password
            reply.CreateDataIDOnly(DataID::LogInDenyWrongPassword);
            out << "Invalid password from client " << remoteEndpoint.IPAddress.toString() << ", " << remoteEndpoint.Port << ".";
        }
    }
    std::cout << out.str() << std::endl;
    send(reply, remoteEndpoint);
    std::cout << "Number of available spot(s): " << (MaxServerCapacity - CurrentPlayers.size()) << "." << std::endl;
}

void ServerSFML::ProcessRequestArena(Endpoint ep)
{
    size_t objectNum = arena.ArenaObjects.size();
}
