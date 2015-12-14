#include "ServerSFML.h"
sf::UdpSocket ServerSFML::socket_;
bool ServerSFML::StopServer;

ServerSFML::ServerSFML(std::string pass, size_t serverCap, size_t teamCap, uint32_t port)
{
    IsServerOnline = false;
    teamA = Team("Team A");
    teamB = Team("Team B");
    playableWidth = 160;
    playableHeight = 80;
    jumpHeight = 20;
    sf::Socket::Status s = socket_.bind(port);

    if (s == sf::Socket::Done)
    {
        IsServerOnline = true;
        socket_.setBlocking(false);
        StopServer = false;
        Password.assign(pass);
        MaxServerCapacity = serverCap;
        MaxTeamCapacity = teamCap;
        arena = Arena(playableWidth, playableHeight, jumpHeight);
        arena.SetupArenaObjects();
        BossUFO bossUFO(playableWidth, playableHeight);
        teamA.Members.push_back(boost::make_shared<BossUFO>(playableWidth, playableHeight));


        //v.push_back(boost::make_shared<B>());
    }
    else if (s == sf::Socket::Error)
    {
        Log("Error in setup server socket. Server is not started.");
    }
    else
    {
        Log("Unexpected server socket status when during setting up. Server is not started.");
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
    Log("Server listens for request...");
    while (StopServer == false)
    {
        doGameUpdate();

        sendDataToClients();

        sf::Packet receivedPacket;
        sf::Socket::Status status = socket_.receive(receivedPacket, sender, port);
        if (status == sf::Socket::Error)
        {
            // error...
            Log("Error in receiving from client.");
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
                Log("Error in server handle_receive:", true);
                Log(e.what(), true);
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
        Log("Serve socket unbinds.");
    }
    catch (std::exception& e)
    {
        Log("Error in server closing socket:", true);
        Log(e.what(), true);
    }
}

void ServerSFML::send(sf::Packet p, Endpoint ep)
{
    try
    {
        sf::Socket::Status status = socket_.send(p, ep.IPAddress, (uint16_t)ep.Port); // &data[0] "convert" vector to array
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
        std::stringstream out;
        out << "Error in server sending data to client " << ep.IPAddress.toString() << "," << ep.Port << ":";
        Log(out.str());
        Log(e.what(), true);
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
            std::stringstream out;
            out << "A player logged out.";
            out << "Number of available spot(s): " << (MaxServerCapacity - CurrentPlayers.size()) << "." << std::endl;
            Log(out.str());
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
    Log(out.str());
    send(reply.p, remoteEndpoint);
    out = std::stringstream();
    out << "Number of available spot(s): " << (MaxServerCapacity - CurrentPlayers.size()) << "." << std::endl;
    Log(out.str());
}

void ServerSFML::ProcessRequestArena(Endpoint ep)
{
    size_t objectTotal = arena.ArenaObjects.size();
    uint8_t totalPackageNeeded = (uint8_t)((objectTotal / Arena::OBJECT_SENT_PER_PACKAGE));
    if (objectTotal % Arena::OBJECT_SENT_PER_PACKAGE != 0)
    {
        totalPackageNeeded++;
    }
    uint8_t packageNum = 1, objectNum = 0;
    sf::Packet p;
    p << (uint8_t)DataID::ArenaInfo << (sf::Uint16)arena.PlayableWidth << (sf::Uint16)arena.PlayableHeight << (sf::Uint16)arena.JumpHeight << (sf::Uint16)objectTotal;
    send(p, ep);
    for (std::map<Point2D, ObjectType>::iterator it = arena.ArenaObjects.begin(); it != arena.ArenaObjects.end(); ++it)
    {
        if (objectNum == 0)
        {
            p = sf::Packet();
            // Format
            // DataID + packageNum + total package num + object num + point1.X + point1.Y + type + point2.X + point2.Y + type + ......
            p << (uint8_t)DataID::ArenaObjects << packageNum << totalPackageNeeded;
            if (packageNum == totalPackageNeeded)
            {
                p << (uint8_t)(objectTotal - (packageNum*Arena::OBJECT_SENT_PER_PACKAGE));
            }
            else
            {
                p << Arena::OBJECT_SENT_PER_PACKAGE;
            }
        }

        p << it->first.X << it->first.Y << (uint8_t)it->second;

        objectNum++;
        if (objectNum == Arena::OBJECT_SENT_PER_PACKAGE)
        {
            objectNum = 0;
            packageNum++;
            send(p, ep);
        }
    }
    if (p.getDataSize() > 4) // dataid + packageNum + totalpackge + object num
    {
        send(p, ep);
    }
}

void ServerSFML::Log(std::string message, bool isError)
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

void ServerSFML::doGameUpdate()
{
    teamA.Members.at(0)->move();
    //std::cout << teamA.Members.at(0)->Pos.X << ", ";
}

void ServerSFML::sendDataToClients()
{
}
