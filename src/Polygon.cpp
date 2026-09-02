#include "Polygon.h"
#include "Point.h"
#include "Color.h"
#include "Rectangle.h"
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

void Polygon::drawWithPivot(Point pivot) {
    std::list<Point> transformedPoints = transform(this->points, pivot);


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

void Polygon::drawBoundary(Color color) {
    std::list<Point> transformedPoints = transform(this->points, this->points.front());
    std::list<Point>::iterator itlp = transformedPoints.begin();

    int maxX, maxY, minX, minY, size, i = 0;
    maxX = minX = transformedPoints.front().getX();
    maxY = minY = transformedPoints.front().getY();
    size = transformedPoints.size();
    for (i = 0; i < size; i++) {
        if (itlp->getX() > maxX) {
            maxX = itlp->getX();
        }
        if (itlp->getX() < minX) {
            minX = itlp->getX();
        }
        if (itlp->getY() > maxY) {
            maxY = itlp->getY();
        }
        if (itlp->getY() < minY) {
            minY = itlp->getY();
        }
        std::advance(itlp, 1);
    }

    Rectangle rect = Rectangle(Point(minX, minY), Point(maxX, maxY), color);
    rect.setRotation(this->getRotation());
    rect.setTranslation(this->getTranslation());
    rect.setScale(this->getScale()[0], this->getScale()[1]);
    rect.drawWithPivot(this->points.front());

    addMouseClickAnchors(Point(minX, minY), Point(maxX, maxY), this->points.front(), color);
}

bool Polygon::isInBoundary(Point point) {
    std::list<Point> transformedPoints = transform(this->points, this->points.front());
    std::list<Point>::iterator itlp = transformedPoints.begin();

    int maxX, maxY, minX, minY, size, i = 0;
    maxX = minX = transformedPoints.front().getX();
    maxY = minY = transformedPoints.front().getY();
    size = transformedPoints.size();
    for (i = 0; i < size; i++) {
        if (itlp->getX() > maxX) {
            maxX = itlp->getX();
        }
        if (itlp->getX() < minX) {
            minX = itlp->getX();
        }
        if (itlp->getY() > maxY) {
            maxY = itlp->getY();
        }
        if (itlp->getY() < minY) {
            minY = itlp->getY();
        }
        std::advance(itlp, 1);
    }

    return point.getX() >= minX && point.getX() <= maxX && point.getY() >= minY && point.getY() <= maxY;
}