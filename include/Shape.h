#ifndef SHAPE_H
#define SHAPE_H
#include <Point.h>
#include <list>

class Shape
{
    public:
        Shape();
        Shape(Point translation, float scale, float rotation);

        void setTranslation(Point translation);
        void setRotation(float rotation);
        void setScale(float scale);

        Point getTranslation();
        float getRotation();
        float getScale();

        // Abstract interface
        virtual void draw() = 0;

        virtual ~Shape();

    protected:
        std::list<Point> transform(std::list<Point> points);

    private:
        float scale = 1.0f;
        float rotation = 0.0f;
        Point translation;

};

#endif // SHAPE_H
