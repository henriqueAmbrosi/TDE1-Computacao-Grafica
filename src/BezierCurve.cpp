#include "BezierCurve.h"
#include "Point.h"
#include "Color.h"
#include "Context.h"
#include <list>
#include <algorithm>

BezierCurve::BezierCurve(std::list<Point> points, Color color)
{
    this->points = points;
    this->color = color;
}

BezierCurve::~BezierCurve()
{
    //dtor
}

void BezierCurve::setPixel(int x, int y, Color color) {
    this->setPixel(x, y, color.getR(), color.getG(), color.getB());
}

void BezierCurve::setPixel(int x, int y, int r, int g, int b) {
    setPixel(x, y, r, g, b, 255);
}

void BezierCurve::setPixel(int x, int y, int r, int g, int b, int a) {
    SDL_Surface* window_surface = Context::getInstance()->getWindowSurface();
    if (!window_surface) return;

    if (x < 0 || x >= window_surface->w || y < 0 || y >= window_surface->h) {
        return;
    }

    unsigned int* pixels = (unsigned int*)window_surface->pixels;
    pixels[x + y * window_surface->w] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

void BezierCurve::draw() {
    std::list<Point> localPoints = this->points;
    std::list<Point> transformedPoints = transform(localPoints);
    std::list<Point>::iterator itlp = transformedPoints.begin();

    Point p1 = *itlp;
    std::advance(itlp, 1);
    Point p2 = *itlp;
    std::advance(itlp, 1);
    Point p3 = *itlp;
    std::advance(itlp, 1);
    Point p4 = *itlp;

    int xu, yu;
    float u;

    for (u = 0; u <= 1; u += 0.001) {
        xu = (int) (pow(1-u, 3)*p1.getX()+3*u*pow(1-u, 2)*p2.getX()+3*pow(u, 2)*(1-u)*p3.getX()+pow(u,3)*p4.getX());
        yu = (int) (pow(1-u, 3)*p1.getY()+3*u*pow(1-u, 2)*p2.getY()+3*pow(u, 2)*(1-u)*p3.getY()+pow(u,3)*p4.getY());
        setPixel(xu, yu, color);
    }
}

void BezierCurve::drawBoundary(Color color) {
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

bool BezierCurve::isInBoundary(Point point) {
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

void BezierCurve::getLocalSize(float& width, float& height)
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