#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "Point.h"
#include "Shape.h"
#include "Color.h"
#include <list>

class BezierCurve : public Shape
{
    public:
        BezierCurve(std::list<Point> points, Color color);
        virtual ~BezierCurve();
        void draw() override;
        bool isInBoundary(Point point);
        void drawBoundary(Color color);
        void setPixel(int x, int y, Color color);
        void setPixel(int x, int y, int r, int g, int b);
        void setPixel(int x, int y, int r, int g, int b, int a);

    protected:

    private:
        std::list<Point> points;
        Color color;
};

#endif // BEZIERCURVE_H
