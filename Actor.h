#ifndef ACTOR_H
#define ACTOR_H


class Actor
{
public:
    enum Type
    {
        Player,
        Spectator,
        Boss
    };
    Type GetType()
    {
        return type;
    }
    void SetType(Type t)
    {
        type = t;
    }
protected:
    Type type;
};

#endif

