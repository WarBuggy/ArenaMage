#include "ServerSFML.h"
sf::UdpSocket ServerSFML::socket_;
bool ServerSFML::StopServer;
std::vector<boost::shared_ptr<Projectile>> Projectile::Projectiles;
extern std::mutex mutex;

ServerSFML::ServerSFML(std::string pass, size_t serverCap, size_t teamCap, uint32_t port)
{
	IsServerOnline = false;
	teamA = Team("Team A");
	teamB = Team("Team B");
	//Projectiles.clear();
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

		// Testing, will be changed
		BossUFO bossUFO(playableWidth, playableHeight);
		bossUFO.team = &teamB;
		teamA.Members.push_back(boost::make_shared<BossUFO>(playableWidth, playableHeight));
		//
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
	double overAmount = 0;
	double msPerCycle = 1000 / FramePerSecond;

	size_t over = 0;
	double overAmountTest = 0;
	sf::Clock overClock;
	overClock.restart();

	while (StopServer == false)
	{
		sf::Time elapsed = clock.restart();
		overAmount = elapsed.asMilliseconds() - msPerCycle;
		if (overAmount <= 0)
		{
			overAmount = 0;
		}
		else
		{
			overAmountTest = overAmountTest + overAmount;
			over++;
			if (over > 100)
			{
				std::cout << overAmountTest << ", " << overClock.getElapsedTime().asMilliseconds() << ", " << Projectile::Projectiles.size() << std::endl;
				over = 0;
				overAmountTest = 0;
				overClock.restart();
			}
		}

		doGameUpdate(elapsed.asMilliseconds());

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
		elapsed = clock.getElapsedTime();
		double temp = msPerCycle - elapsed.asMilliseconds();
		temp = std::min(temp, msPerCycle);
		temp = temp - overAmount;
		if (temp > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((long)temp));
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
	else if (id == DataID::ArenaReceivedCompleted)
	{
		CurrentPlayers[ep] = ActorType::SPECTATOR;
	}
}

void ServerSFML::ProcessLogOutPacket(Packet p, Endpoint remoteEndpoint)
{
	for (std::map<Endpoint, ActorType>::iterator it = CurrentPlayers.begin(); it != CurrentPlayers.end(); ++it)
	{
		if ((it->first) == remoteEndpoint)
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
				CurrentPlayers.insert(std::pair<Endpoint, ActorType>(remoteEndpoint, ActorType::UNKNOWN));
				reply.CreateDataIDOnly(DataID::LogInServerAccept);

				//size_t nameLength = data[1 + passwordLength];
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
	out << "Number of available spot(s): " << (MaxServerCapacity - CurrentPlayers.size()) << ".";
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
	p << (sf::Uint8)DataID::ArenaInfo << (sf::Uint16)arena.PlayableWidth << (sf::Uint16)arena.PlayableHeight << (sf::Uint16)arena.JumpHeight << (sf::Uint16)objectTotal;
	send(p, ep);
	for (std::map<Point2D, ObjectType>::iterator it = arena.ArenaObjects.begin(); it != arena.ArenaObjects.end(); ++it)
	{
		if (objectNum == 0)
		{
			p = sf::Packet();
			// Format
			// DataID + packageNum + total package num + object num + point1.X + point1.Y + type + point2.X + point2.Y + type + ......
			p << (sf::Uint8)DataID::ArenaObjects << packageNum << totalPackageNeeded;
			if (packageNum == totalPackageNeeded)
			{
				p << (uint8_t)(objectTotal - (packageNum*Arena::OBJECT_SENT_PER_PACKAGE));
			}
			else
			{
				p << Arena::OBJECT_SENT_PER_PACKAGE;
			}
		}

		p << it->first.X << it->first.Y << (sf::Uint8)it->second;

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
	std::unique_lock<std::mutex> lock(mutex);
	if (!isError)
	{
		std::cout << message << std::endl;
	}
	else
	{
		std::cerr << message << std::endl;
	}
	lock.unlock();
}

void ServerSFML::doGameUpdate(sf::Uint32 elapsed)
{
	teamA.Members.at(0)->move(elapsed);
	teamA.Members.at(0)->attack();
	for (std::vector<int>::size_type i = 0; i != Projectile::Projectiles.size(); i++)
	{
		Projectile::Projectiles.at(i)->Move(elapsed);
	}
}

void ServerSFML::sendDataToClients()
{
	sf::Packet actorsPacket, projectilesPacket;
	// Prepare actors packet
	actorsPacket << (sf::Uint8)DataID::ActorInfo;
	actorsPacket << (sf::Uint8) 1; // number of actor
	actorsPacket << (sf::Uint8) ActorID::BOSS_UFO;
	actorsPacket << (sf::Uint16) teamA.Members.at(0)->width << (sf::Uint16) teamA.Members.at(0)->height;
	actorsPacket << (float)teamA.Members.at(0)->Pos.X << (float)teamA.Members.at(0)->Pos.Y;
	// Prepare projectiles packet
	sf::Uint8 projectilesNum = (sf::Uint8)Projectile::Projectiles.size();
	if (projectilesNum > 0)
	{
		projectilesPacket << (sf::Uint8) DataID::ProjectileInfo << projectilesNum;
		for (std::vector<int>::size_type i = 0; i != Projectile::Projectiles.size(); i++)
		{
			Projectile::Projectiles.at(i)->CreateDataForClients(projectilesPacket);
		}
	}
	// send to all clients who already authented
	for (std::map<Endpoint, ActorType>::iterator it = CurrentPlayers.begin(); it != CurrentPlayers.end(); ++it)
	{
		if (it->second != ActorType::UNKNOWN)
		{
			send(actorsPacket, it->first);
			if (projectilesNum > 0)
			{
				send(projectilesPacket, it->first);
			}
		}
	}
}
