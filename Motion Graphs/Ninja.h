#ifndef _NINJA_H_
#define _NINJA_H_

#include "OgreAnimation.h"

using namespace Ogre;

class Ninja : public Animation
{
private:
    Motion *motion;
}

class PointCloud
{
private:
    Point *p;
    int nPoint;
}

class Motion
{
private:
    PointCloud * clouds;
    int nClouds;
    char * label;
}

class Point
{
public:
    float x,y,z;
}

#endif