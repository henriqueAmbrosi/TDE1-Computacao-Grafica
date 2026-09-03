#include "Icon.h"
#include "Rectangle.h"
#include "Line.h"
#include "Circle.h"
#include "Polygon.h"
#include "Paint.h"
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
        case IconType::COLOR:
            this->drawColorIcon(center);
            break;
        case IconType::SAVE:
            this->drawSaveIcon(center);
            break;
    }
}

void Icon::drawColorIcon(Point center)
{
    int size = 8;
    int cx = center.getX();
    int cy = center.getY();
    Rectangle rect(Point(cx - size, cy - size), Point(cx + size, cy + size), this->color);
    rect.draw();
    Paint p = Paint();
    p.fill(cx, cy, this->color);
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

void Icon::drawCurveIcon(Point center)
{
    int cx = center.getX();
    int cy = center.getY();

    std::list<Point> curvePoints = {
        Point(cx - 6, cy + 3),
        Point(cx - 2, cy - 9),
        Point(cx + 2, cy + 9),
        Point(cx + 6, cy - 3)
    };

    BezierCurve curve(curvePoints, this->color);
    curve.draw();
}

void Icon::drawPaintIcon(Point center)
{
    int cx = center.getX();
    int cy = center.getY();

    Rectangle bucket(Point(cx - 8, cy - 5), Point(cx - 1, cy + 4), this->color);
    bucket.setRotation(45);
    bucket.drawWithPivot(Point(cx - 4, cy - 1));

    Point dropTop(cx + 3, cy + 1);
    Point dropBottom(cx + 3, cy + 7);

    std::list<Point> leftCurvePoints = {
        dropTop,
        Point(cx + 0, cy + 3),
        Point(cx + 0, cy + 6),
        dropBottom
    };
    BezierCurve leftCurve(leftCurvePoints, this->color);
    leftCurve.draw();

    std::list<Point> rightCurvePoints = {
        dropTop,
        Point(cx + 6, cy + 3),
        Point(cx + 6, cy + 6),
        dropBottom
    };
    BezierCurve rightCurve(rightCurvePoints, this->color);
    rightCurve.draw();
}

void Icon::drawSaveIcon(Point center)
{
    int cx = center.getX();
    int cy = center.getY();
    int size = 7;

    Rectangle body(Point(cx - size, cy - size), Point(cx + size, cy + size), this->color);
    body.draw();

    Rectangle topNotch(Point(cx - 4, cy - size), Point(cx + 2, cy - 2), this->color);
    topNotch.draw();

    Rectangle labelArea(Point(cx - 4, cy + 1), Point(cx + 4, cy + size), this->color);
    labelArea.draw();
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