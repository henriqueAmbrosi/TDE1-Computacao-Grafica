#ifndef FLOODFILL_H
#define FLOODFILL_H

#include <SDL2/SDL.h>
#include "Color.h"
#include "Point.h"
#include "Drawable.h"

class FloodFill : public Drawable
{
public:
    FloodFill();
    FloodFill(Point p, Color c);
    virtual ~FloodFill();

    void draw() override;
    void fill(int x, int y, Color fillColor);
    void fill(Point point, Color fillColor);

    Uint32 getPixel(int x, int y);
    void setPixel(int x, int y, Uint32 pixelColor);
    void setPixel(int x, int y, Color color);
    void setPixel(int x, int y, int r, int g, int b, int a = 255);

private:
    Color fillColor;
    Point fillPoint;
};

#endif // FLOODFILL_H