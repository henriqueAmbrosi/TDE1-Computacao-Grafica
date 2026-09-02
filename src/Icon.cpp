#include "Icon.h"
#include "Rectangle.h"
#include "Line.h"
#include "Circle.h"
#include "Polygon.h"
#include "BezierCurve.h"
#include <list>

Icon::Icon()
{
    this->type = IconType::RECTANGLE;
    this->color = Color(0, 0, 0);
    this->center = Point(0, 0);
}

Icon::Icon(IconType type, Color color){
    this->type = type;
    this->color = color;
    this->center = Point(0, 0);
}

Icon::Icon(IconType type, Color color, Point center)
{
    this->type = type;
    this->color = color;
    this->center = center;
}

Icon::~Icon()
{
}

void Icon::draw(){
    drawAt(this->center);
}

void Icon::drawAt(Point center)
{
    this->center = center;

    switch (this->type)
    {
        case IconType::RECTANGLE:
            this->drawRectangleIcon(center);
            break;
        case IconType::LINE:
            this->drawLineIcon(center);
            break;
        case IconType::CIRCLE:
            this->drawCircleIcon(center);
            break;
        case IconType::CURVE:
            this->drawCurveIcon(center);
            break;
        case IconType::POLYGON:
            this->drawPolygonIcon(center);
            break;
        case IconType::SELECT:
            this->drawSelectIcon(center);
            break;
        case IconType::PAINT:
            this->drawPaintIcon(center);
            break;
    }
}

void Icon::drawRectangleIcon(Point center)
{
    int size = 8;
    int cx = center.getX();
    int cy = center.getY();
    Rectangle rect(Point(cx - size, cy - size), Point(cx + size, cy + size), this->color);
    rect.draw();
}

void Icon::drawLineIcon(Point center)
{
    int size = 8;
    int cx = center.getX();
    int cy = center.getY();
    Line line(Point(cx - size, cy + size), Point(cx + size, cy - size), this->color);
    line.draw();
}

void Icon::drawCircleIcon(Point center)
{
    int size = 8;
    Circle circle(center, size, this->color);
    circle.draw();
}

void Icon::drawPolygonIcon(Point center)
{
    int cx = center.getX();
    int cy = center.getY();

    std::list<Point> starPoints = {
        Point(cx, cy - 8), 
        Point(cx + 2, cy - 2), 
        Point(cx + 8, cy - 2),
        Point(cx + 3, cy + 2),
        Point(cx + 5, cy + 8),
        Point(cx, cy + 4),
        Point(cx - 5, cy + 8),
        Point(cx - 3, cy + 2),
        Point(cx - 8, cy - 2),
        Point(cx - 2, cy - 2) 
    };

    Polygon star(starPoints, this->color);
    star.draw();
}

void Icon::drawSelectIcon(Point center)
{
    int cx = center.getX();
    int cy = center.getY();

    std::list<Point> points = {
        Point(cx - 4, cy - 7),
        Point(cx - 4, cy + 5),
        Point(cx - 1, cy + 2),
        Point(cx + 2, cy + 7),
        Point(cx + 4, cy + 6),
        Point(cx + 1, cy + 1),
        Point(cx + 5, cy + 1) 
    };

    Polygon pointer(points, this->color);
    pointer.draw();
}

void Icon::drawCurveIcon(Point center) {
    int cx = center.getX();
    int cy = center.getY();

    std::list<Point> points = {
        Point(cx - 4, cy - 7),
        Point(cx - 4, cy + 5),
        Point(cx - 1, cy + 2),
        Point(cx + 2, cy + 7)
    };

    BezierCurve b = BezierCurve(points, color);
    b.draw();
}

void Icon::drawPaintIcon(Point center)
{
    int size = 4;
    Circle bucket(center, size, this->color);
    bucket.draw();
}

IconType Icon::getType()
{
    return this->type;
}

void Icon::setType(IconType type)
{
    this->type = type;
}

void Icon::setColor(Color color)
{
    this->color = color;
}