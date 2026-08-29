#include "Polygon.h"
#include "Point.h"
#include "Color.h"
#include "Line.h"
#include <list>

Polygon::Polygon(std::list<Point> points, Color color)
{
    this->points = points;
    this->color = color;
}

Polygon::~Polygon()
{
    //dtor
}

void Polygon::draw()
{
    std::list<Point> transformedPoints = transform(this->points);


    std::list<Line> localLines = {};
    std::list<Point>::iterator itlp = transformedPoints.begin();
    std::list<Point>::iterator itlp2 = transformedPoints.begin();
    std::list<Line>::iterator itll = localLines.begin();
    std::advance(itlp2, 1);

    int size, i = 0;
    size = transformedPoints.size() - 1;
    for (i = 0; i < size; i++) {
        localLines.insert(itll, Line(*itlp, *itlp2, this->color));
        std::advance(itlp, 1);
        std::advance(itlp2, 1);
        std::advance(itll, 1);
    }

    itlp2 = transformedPoints.begin();
    localLines.insert(itll, Line(*itlp, *itlp2, this->color));

    itll = localLines.begin();
    for (i = 0; i < size; i++) {
        (*itll).draw();
        std::advance(itll, 1);
    }
    (*itll).draw();
}