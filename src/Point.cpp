#include "Point.h"

Point::Point()
{
    //ctor
}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
    this->z = 0;
}

Point::~Point()
{
    //dtor
}


int Point::getX()
{
    return this->x;
}

int Point::getY()
{
    return this->y;
}

int Point::getZ()
{
    return this->z;
}

void Point::setX(int x)
{
    this->x = x;
}

void Point::setY(int y)
{
    this->y = y;
}

void Point::setZ(int z)
{
    this->z = z;
}

