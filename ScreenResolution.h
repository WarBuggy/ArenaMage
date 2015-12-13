#ifndef SCREENRESOLUTION_H
#define SCREENRESOLUTION_H

struct ScreenResolution
{
    size_t Width;
    size_t Height;

    bool operator ==(const ScreenResolution& sr) const
    {
        return (sr.Width == Width && sr.Height == Height);
    }

    bool operator <(const ScreenResolution &sr) const
    {
        if (Width < sr.Width)
        {
            return true;
        }
        else if (Width == sr.Width)
        {
            if (Height < sr.Height)
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