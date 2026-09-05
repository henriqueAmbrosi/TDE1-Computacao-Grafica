#ifndef SHAPE_H
#define SHAPE_H
#include "Point.h"
#include "Color.h"
#include "Drawable.h"
#include <list>

enum class Anchor {
    NONE = -1,
    TOP_LEFT = 0,
    TOP_RIGHT = 1,
    BOTTOM_LEFT = 2,
    BOTTOM_RIGHT = 3
};

class Rectangle;

class Shape: public Drawable
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
        void addMouseClickAnchorsUntransformed(Point start, Point end, Point pivot, Color color);
        virtual void getLocalSize(float& width, float& height);
        Anchor inAnchors(Point point);
        
        // Abstract interface
        virtual void draw() override = 0;
        virtual void drawBoundary(Color color) = 0;
        virtual bool isInBoundary(Point point) = 0;
        
        virtual ~Shape();

    protected:
        std::list<Point> transform(std::list<Point>& points);
        std::list<Point> transform(std::list<Point>& points, Point pivot);
        std::list<Point> getBoundaryPointsList(Point start, Point end, Point pivot);
        void addMouseClickAnchors(Point start, Point end, Point pivot, Color color);

    private:
        float scale[2] = { 1.0f, 1.0f };
        float rotation = 0.0f;
        std::list<Rectangle> anchors;
        Point translation;

        double toRadians(double degrees);
        float rotateAndScaleMatrix[3][3];
        void buildRotateAndScaleMatrix();
        void multiplyMatrix3x3(float A[3][3], float B[3][3], float result[3][3]);
};

#endif // SHAPE_H