#include "ClientSFML.h"
sf::UdpSocket ClientSFML::socket_;
bool ClientSFML::StopClient;
extern std::mutex mutex;
std::vector<boost::shared_ptr<Projectile>> ClientSFML::ClientProjectiles;

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
		out << "Authen complete.";
		Log(out.str());
		State = STATE::AuthenCompleted;
		Packet p;
		p.CreateDataIDOnly(DataID::RequestArenaInfo);
		send(p);
	}
	else if (p.ID == DataID::LogInDenyWrongPassword)
	{
		out << "Wrong password. Authen failed.";
		Log(out.str());
	}
	else if (p.ID == DataID::LogInDenyMaxPlayer)
	{
		out << "Server is full. Authen failed.";
		Log(out.str());
	}
	else if (p.ID == DataID::ArenaObjects)
	{
		ProcessArenaPackages(p);
	}
	else if (p.ID == DataID::ArenaInfo)
	{
		ProcessArenaInfoPackage(p);
	}
	else if (p.ID == DataID::ActorInfo)
	{
		ProcessActorInfoPacket(p);
	}
	else if (p.ID == DataID::ProjectileInfo)
	{
		ProcessProjectileInfoPacket(p);
	}
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
		//p << (sf::Uint8)DataID::RequestArenaInfo;
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
				sf::Uint8 type;
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
						sf::Packet reply;
						reply << (sf::Uint8)DataID::ArenaReceivedCompleted;
						send(reply);

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

void ClientSFML::ProcessActorInfoPacket(Packet p)
{
	std::unique_lock<std::mutex> lock(mutex);
	sf::Uint8 actorNum = 0;
	p.p >> actorNum;
	Actors.clear();
	for (sf::Uint8 i = 0; i < actorNum; i++)
	{
		sf::Uint8 actorID = 0;
		sf::Uint16 w = 0, h = 0;
		float x = 0, y = 0;
		bool success = p.p >> actorID >> w >> h >> x >> y;
		if (success)
		{
			Actor a(x, y);
			a.setSize(w, h);
			a.id = (ActorID)actorID;
			Actors.push_back(a);
		}
	}
	lock.unlock();
}

void ClientSFML::ProcessProjectileInfoPacket(Packet p)
{
	std::unique_lock<std::mutex> lock(mutex);
	sf::Uint8 proNum = 0;
	p.p >> proNum;
	ClientProjectiles.clear();
	for (sf::Uint8 i = 0; i < proNum; i++)
	{
		sf::Uint8 proID = 0;
		float x = 0, y = 0, rotation = 0;
		bool success = p.p >> proID >> x >> y >> rotation;
		if (success)
		{
			if (proID == ObjectType::PROJECTILE_BLASTER)
			{
				ProjectileBlaster pro(x, y, rotation);
				ClientProjectiles.push_back(boost::make_shared<ProjectileBlaster>(pro));
			}
		}
	}
	lock.unlock();
}

void ClientSFML::DrawActors(sf::RenderWindow & window, float offsetX, float offsetY)
{
	std::unique_lock<std::mutex> lock(mutex);
	for (std::vector<Actor>::iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		sf::RectangleShape rec;
		rec.setSize(sf::Vector2f((float)it->width*it->SCALE, (float)it->height*it->SCALE));
		rec.setPosition(it->Pos.X * it->SCALE + offsetX, it->Pos.Y * it->SCALE + offsetY);
		if (it->id == (sf::Uint8)  ActorID::BOSS_UFO)
		{
			rec.setFillColor(sf::Color(50, 205, 50));
		}
		window.draw(rec);
	}
	if (ClientProjectiles.size() > 0)
	{
		size_t scale = ClientProjectiles.at(0)->SCALE;
		for (std::vector<int>::size_type i = 0; i != ClientProjectiles.size(); i++)
		{
			sf::RectangleShape rec;
			rec.setSize(sf::Vector2f((float)ClientProjectiles.at(i)->width*scale, (float)ClientProjectiles.at(0)->height*scale));
			rec.setPosition(ClientProjectiles.at(i)->Pos.X * scale + offsetX, ClientProjectiles.at(i)->Pos.Y * scale + offsetY);
			rec.setFillColor(ClientProjectiles.at(i)->Color);
			rec.setRotation(ClientProjectiles.at(i)->Rotation);
			window.draw(rec);
		}
	}
	lock.unlock();
}