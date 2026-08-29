#ifndef POLYGON_H
#define POLYGON_H
#include <Point.h>
#include <Color.h>
#include <Shape.h>
#include <list>


class Polygon : public Shape
{
    public:
        Polygon(std::list<Point> points, Color color);
        virtual ~Polygon();
        void draw() override;

    protected:
        std::list<Point> points;
        Color color;

    private:
};

#endif // POLYGON_H
