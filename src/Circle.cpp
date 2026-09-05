#include "Circle.h"
#include "Line.h"
#include "Context.h"
#include "Rectangle.h"
#include "Color.h"


Circle::Circle(Point center, int radius, Color color)
{
    this->center = center;
    this->radius = radius;
    this->color = color;
}

Circle::~Circle()
{
    //dtor
}

void Circle::setPixel(int x, int y, Color color) {
    Uint8 r = color.getR();
    Uint8 g = color.getG();
    Uint8 b = color.getB();
    this->setPixel(x, y, r, g, b);
}

void Circle::setPixel(int x, int y, int r, int g, int b) {
    setPixel(x, y, r, g, b, 255);
}

void Circle::setPixel(int x, int y, int r, int g, int b, int a) {
    SDL_Surface* window_surface = Context::getInstance()->getWindowSurface();
    if (!window_surface) return;

    if (x < 0 || x >= window_surface->w || y < 0 || y >= window_surface->h) {
        return;
    }

    unsigned int* pixels = (unsigned int*)window_surface->pixels;
    pixels[x + y * window_surface->w] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

void Circle::displayBresenhamCircle(int xc,int yc, int x, int y, Color color) {
    // desenha todas as 8 coordenadas de(x,y),
    // uma para cada octante
    setPixel(xc+x, yc+y, color);
    setPixel(xc-x, yc+y, color);
    setPixel(xc+x, yc-y, color);
    setPixel(xc-x, yc-y, color);
    setPixel(xc+y, yc+x, color);
    setPixel(xc-y, yc+x, color);
    setPixel(xc+y, yc-x, color);
    setPixel(xc-y, yc-x, color);
}

void Circle::draw() {
    int scaledRadius = (int) radius * this->getScale()[0];
    int x = 0, y = scaledRadius;
    int decisionParameter = 3 - 2 * scaledRadius;

    std::list<Point> localPoints = { this->center };
    std::list<Point> transformedPoints = transform(localPoints, this->center);
    std::list<Point>::iterator itlp = transformedPoints.begin();

    displayBresenhamCircle((*itlp).getX(), (*itlp).getY(), x, y, color);
 
    while (y >= x) {
        x++;
        if (decisionParameter > 0) {
            y--;
            decisionParameter = decisionParameter + 4 * (x - y) + 10;
        } else {
            decisionParameter = decisionParameter + 4 * x + 6;
        }
        
        displayBresenhamCircle((*itlp).getX(), (*itlp).getY(), x, y, color);
    }
}

void Circle::drawBoundary(Color color) {
    Point start, end;
    start.setX(this->center.getX() - this->radius);
    start.setY(this->center.getY() - this->radius);

    end.setX(this->center.getX() + this->radius);
    end.setY(this->center.getY() + this->radius);

    Rectangle rect = Rectangle(start, end, color);
    rect.setRotation(this->getRotation());
    rect.setTranslation(this->getTranslation());
    rect.setScale(this->getScale()[0], this->getScale()[1]);
    rect.drawWithPivot(this->center);
    
    addMouseClickAnchors(start, end, this->center, color);
}

bool Circle::isInBoundary(Point point) {
    std::list<Point> localPoints = { this->center };
    std::list<Point> transformedPoints = transform(localPoints, this->center);
    std::list<Point>::iterator itlp = transformedPoints.begin();

    return sqrt(pow(point.getX() - (*itlp).getX(), 2) + pow(point.getY() - (*itlp).getY(), 2)) <= this->radius * this->getScale()[0];
}

void Circle::getLocalSize(float& width, float& height)
{
    width = (float) (this->radius * 2);
    height = (float) (this->radius * 2);
}