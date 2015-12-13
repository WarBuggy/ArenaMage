#ifndef ENDPOINT_H
#define ENDPOINT_H

#include "SFML/Network.hpp"

struct Endpoint
{
    sf::IpAddress IPAddress;
    size_t Port;

    bool operator ==(const Endpoint& p) const
    {
        return (p.Port == Port && p.IPAddress == IPAddress);
    }

    bool operator <(const Endpoint &p) const
    {
        if (IPAddress < p.IPAddress)
        {
            return true;
        }
        else if (IPAddress == p.IPAddress)
        {
            if (Port < p.Port)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
};



#endif