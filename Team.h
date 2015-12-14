#ifndef TEAM_H
#define TEAM_H

#include "Boss.h"

class Team
{
public:
    Team() {}
    Team(std::string name)
    {
        Name = name;
    }
    std::string Name;
    std::vector<boost::shared_ptr<Boss>> Members;
};


#endif
