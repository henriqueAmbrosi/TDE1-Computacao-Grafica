#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Color.h"

class Circle : public Shape
{
    public:
        Circle(Point center, int radius, Color color);
        void setPixel(int x, int y, Color color);
        void setPixel(int x, int y, int r, int g, int b);
        void setPixel(int x, int y, int r, int g, int b, int a);
        void draw() override;
        bool isInBoundary(Point point) override;
        void getLocalSize(float& width, float& height) override;
        void drawBoundary(Color color) override;
        virtual ~Circle();

    protected:

    private:
        Point center;
        int radius;
        Color color;
        void displayBresenhamCircle(int xc,int yc, int x, int y, Color color);
};

#endif // CIRCLE_H
