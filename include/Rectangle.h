#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <Point.h>
#include <Color.h>
#include <Shape.h>

class Rectangle : public Shape
{
    public:
        Rectangle();
        Rectangle(Point start, Point end, Color color);
        Rectangle(Point start, Point end, Color color, int antialias);
        void draw() override;
        void drawBoundary(Color color) override;
        bool isInBoundary(Point point) override;
        void drawWithPivot(Point pivot);
        virtual ~Rectangle();


    protected:

    private:
        Point start;
        Point end;
        Color color;
        int antialias = 0;
        Uint32 getPixel(int x, int y);
        std::list<Point> getBoundaryPointsList(Point start, Point end);
        void addMouseClickAnchors(Point start, Point end, Color color);
};

#endif // RECTANGLE_H
