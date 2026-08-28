#include "Shape.h"

Shape::Shape()
{
    this->scale = 1.0f;
    this->rotation = 0.0f;
}

Shape::Shape(Point translation, float scale, float rotation)
{
    this->translation = translation;
    this->scale = scale;
    this->rotation = rotation;
}

Shape::~Shape()
{
    //dtor
}

void Shape::setTranslation(Point translation)
{
    this->translation = translation;
}

void Shape::setRotation(float rotation)
{
    this->rotation = rotation;
}

void Shape::setScale(float scale)
{
    this->scale = scale;
}

Point Shape::getTranslation()
{
    return this->translation;
}

float Shape::getRotation()
{
    return this->rotation;
}

float Shape::getScale()
{
    return this->scale;
}

std::list<Point> Shape::transform(std::list<Point> points)
{
    std::list<Point> transformedPoints;

    for (Point p : points) {
        

        transformedPoints.push_back(p);
    }

    return transformedPoints;
}