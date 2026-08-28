#include "Rectangle.h"
#include <list>

Rectangle::Rectangle() {
    this->antialias = 0;
}

Rectangle::Rectangle(Point start, Point end, Color color) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->antialias = 0;
}

Rectangle::Rectangle(Point start, Point end, int antialias) {
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

    // 2. Pass local points through Shape's transformation method
    // (applies scale, rotation, and translation)
    std::list<Point> transformedPoints = transform(localPoints);

    // 3. Render the transformed shape edges onto the screen
    // Iterate through transformedPoints and draw lines between adjacent points using your pixel/SDL functions.
}