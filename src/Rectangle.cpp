#include "Rectangle.h"
#include <list>
#include "Line.h"

Rectangle::Rectangle() {
    this->antialias = 0;
}

Rectangle::Rectangle(Point start, Point end, Color color) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->antialias = 0;
}

Rectangle::Rectangle(Point start, Point end, Color color, int antialias) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->antialias = antialias;
}

Rectangle::~Rectangle()
{
}

void Rectangle::draw()
{
    Point p1 = start;
    Point p2 = Point(end.getX(), start.getY());
    Point p3 = end;
    Point p4 = Point(start.getX(), end.getY());

    std::list<Point> localPoints = { p1, p2, p3, p4 };
    std::list<Point> transformedPoints = transform(localPoints);
    std::list<Line> localLines = {};
    std::list<Point>::iterator itlp = transformedPoints.begin();
    std::list<Point>::iterator itlp2 = transformedPoints.begin();
    std::list<Line>::iterator itll = localLines.begin();
    std::advance(itlp2, 1);

    int i = 0;
    for (i = 0; i < 3; i++) {
        localLines.insert(itll, Line(*itlp, *itlp2, this->color));
        std::advance(itlp, 1);
        std::advance(itlp2, 1);
        std::advance(itll, 1);
    }

    itlp2 = transformedPoints.begin();
    localLines.insert(itll, Line(*itlp, *itlp2, this->color));

    itll = localLines.begin();
    for (i = 0; i < 3; i++) {
        (*itll).draw();
        std::advance(itll, 1);
    }
    (*itll).draw();
}