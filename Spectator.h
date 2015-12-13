#ifndef SPECTATOR_H
#define SPECTATOR_H
#include "Actor.h"
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class Spectator : public Actor
{
public:
    Spectator()
    {
        type = Actor::Type::Spectator;
    };
    void SetEndpoint(udp::endpoint e)
    {
        endpoint = e;
    }
    udp::endpoint GetEndPoint()
    {
        return endpoint;
    }
    void SetArenaComplete(bool complete)
    {
        arenaComplete = complete;
    }
    bool GetArenaComplete()
    {
        return arenaComplete;
    }
protected:
    udp::endpoint endpoint;
    bool arenaComplete = false; // if this actor has complete arena info
};

#endif
