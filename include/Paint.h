#ifndef PAINT_H
#define PAINT_H

#include <SDL2/SDL.h>
#include "Color.h"
#include "Point.h"
#include "Drawable.h"

class Paint : public Drawable
{
public:
    Paint();
    Paint(Point p, Color c);
    virtual ~Paint();

    void draw() override;
    void fill(int x, int y, Color fillColor);
    void fill(Point point, Color fillColor);
    void forceFill(Point point, Color fillColor, Color limitColor);

    Uint32 getPixel(int x, int y);
    void setPixel(int x, int y, Uint32 pixelColor);
    void setPixel(int x, int y, Color color);
    void setPixel(int x, int y, int r, int g, int b, int a = 255);


private:
    Color fillColor;
    Point fillPoint;
};

#endif // PAINT_H