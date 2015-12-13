#ifndef POINT2D_H
#define POINT2D_H

struct Point2D
{
    float X;
    float Y;

    bool operator ==(const Point2D& p) const
    {
        return (p.X == X && p.Y == Y);
    }

    bool operator <(const Point2D &p) const
    {
        if (Y < p.Y)
        {
            return true;
        }
        else if (Y == p.Y)
        {
            if (X < p.X)
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
