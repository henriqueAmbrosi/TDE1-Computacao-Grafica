#include "Rectangle.h"
#include <list>
#include "Line.h"
#include "Color.h"

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
    std::list<Point> transformedPoints = transform(localPoints, start);
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

void Rectangle::drawWithPivot(Point pivot)
{
    Point p1 = start;
    Point p2 = Point(end.getX(), start.getY());
    Point p3 = end;
    Point p4 = Point(start.getX(), end.getY());

    std::list<Point> localPoints = { p1, p2, p3, p4 };
    std::list<Point> transformedPoints = transform(localPoints, pivot);
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

std::list<Point> Rectangle::getBoundaryPointsList(Point start, Point end) {
    std::list<Point> localPoints = { start, Point(end.getX(), start.getY()), end, Point(start.getX(), end.getY()) };
    std::list<Point> transformedPoints = transform(localPoints, this->start);
    return transformedPoints;
}

void Rectangle::addMouseClickAnchors(Point start, Point end, Color color) {
    std::list<Rectangle> rects = {};
    std::list<Rectangle>::iterator rI = rects.begin();

    std::list<Point> transformedPoints = getBoundaryPointsList(start, end);
    std::list<Point>::iterator ittp = transformedPoints.begin();

    int i = 0, size = transformedPoints.size();
    for (i = 0; i < size; i++) {
        Point anchorStart = *ittp, anchorEnd = *ittp;
        anchorEnd.setX(anchorEnd.getX() + 5);
        anchorEnd.setY(anchorEnd.getY() + 5);
        anchorStart.setX(anchorStart.getX() - 5);
        anchorStart.setY(anchorStart.getY() - 5);
        rI = rects.insert(rI, Rectangle(anchorStart, anchorEnd, color));
        (*rI).draw();
        // Todo | add flood fill
        std::advance(ittp, 1);
        std::advance(rI, 1);
    }
}

void Rectangle::drawBoundary(Color color)
{
    Point center(
        (start.getX() + end.getX()) / 2,
        (start.getY() + end.getY()) / 2
    );

    const float padding = 1.1f;
    auto inflate = [&](Point p) {
        return Point(
            center.getX() + (int)((p.getX() - center.getX()) * padding),
            center.getY() + (int)((p.getY() - center.getY()) * padding)
        );
    };

    Rectangle rect = Rectangle(inflate(start), inflate(end), color);
    rect.setRotation(this->getRotation());
    rect.setTranslation(this->getTranslation());
    rect.setScale(this->getScale()[0], this->getScale()[1]);
    rect.drawWithPivot(start);

    addMouseClickAnchors(start, end, color);
}

bool Rectangle::isInBoundary(Point point)
{
    return point.getX() >= start.getX() && point.getX() <= end.getX() && point.getY() >= start.getY() && point.getY() <= end.getY();
}