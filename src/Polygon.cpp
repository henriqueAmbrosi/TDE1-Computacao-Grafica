#include "Polygon.h"
#include "Point.h"
#include "Color.h"
#include "Rectangle.h"
#include "Line.h"
#include <list>
#include <algorithm>

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
        localLines.insert(itll, Line(*itlp, *itlp2, this->color, 1));
        std::advance(itlp, 1);
        std::advance(itlp2, 1);
        std::advance(itll, 1);
    }

    itlp2 = transformedPoints.begin();
    localLines.insert(itll, Line(*itlp, *itlp2, this->color, 1));

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
        localLines.insert(itll, Line(*itlp, *itlp2, this->color, 1));
        std::advance(itlp, 1);
        std::advance(itlp2, 1);
        std::advance(itll, 1);
    }

    itlp2 = transformedPoints.begin();
    localLines.insert(itll, Line(*itlp, *itlp2, this->color, 1));

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

    if (this->inAnchors(point) != Anchor::NONE) {
        return true;
    }

    return point.getX() >= minX && point.getX() <= maxX && point.getY() >= minY && point.getY() <= maxY;
}

void Polygon::getLocalSize(float& width, float& height)
{
    if (this->points.empty()) {
        width = 1.0f;
        height = 1.0f;
        return;
    }

    int minX = this->points.front().getX();
    int maxX = minX;
    int minY = this->points.front().getY();
    int maxY = minY;

    for (Point p : this->points) {
        if (p.getX() < minX) minX = p.getX();
        if (p.getX() > maxX) maxX = p.getX();
        if (p.getY() < minY) minY = p.getY();
        if (p.getY() > maxY) maxY = p.getY();
    }

    width = static_cast<float>(std::max(1, maxX - minX));
    height = static_cast<float>(std::max(1, maxY - minY));
}