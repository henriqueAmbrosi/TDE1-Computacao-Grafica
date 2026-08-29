#ifndef SHAPE_H
#define SHAPE_H
#include "Point.h"
#include <list>

class Shape
{
    public:
        Shape();
        Shape(Point translation, float scale[2], float rotation);

        void setTranslation(Point translation);
        void setRotation(float rotation);
        void setScale(float x, float y);

        Point getTranslation();
        float getRotation();
        float* getScale();

        // Abstract interface
        virtual void draw() = 0;

        virtual ~Shape();

    protected:
        std::list<Point> transform(std::list<Point>& points);
        std::list<Point> transform(std::list<Point>& points, Point pivot);

    private:
        float scale[2] = { 1.0f, 1.0f };
        float rotation = 0.0f;
        Point translation;

        double toRadians(double degrees);
        float rotateAndScaleMatrix[3][3];
        void buildRotateAndScaleMatrix();
        void multiplyMatrix3x3(float A[3][3], float B[3][3], float result[3][3]);
};

#endif // SHAPE_H