#ifndef LINE_H
#define LINE_H

#include "Shape.h"
#include "Point.h"
#include "Color.h"
#include <SDL2/SDL.h>

class Line : public Shape
{
public:
    Line();
    Line(Point start, Point end, Color color);
    Line(Point start, Point end, Color color, int antialias);
    virtual ~Line();

    void setPixel(int x, int y, Uint32 cor);
    void setPixel(int x, int y, Color color);
    void setPixel(int x, int y, int r, int g, int b);
    void setPixel(int x, int y, int r, int g, int b, int a);
    Uint32 getPixel(int x, int y);

    void drawWuLine(int x0, int y0, int x1, int y1, Color color);
    void bresenham(int x1, int y1, int x2, int y2, int r, int g, int b);

    void draw() override;
    void drawBoundary(Color color) override;
    bool isInBoundary(Point point) override;
    void getLocalSize(float& width, float& height) override;

    Point getStart() { return start; }
    Point getEnd() { return end; }

private:
    Point start;
    Point end;
    Color color;
    int antialias = 0;
};

#endif // LINE_H