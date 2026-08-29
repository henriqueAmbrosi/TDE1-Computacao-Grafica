#include "Shape.h"
#include <cmath>
#include <iostream>

Shape::Shape()
{
    this->scale[0] = 1.0f;
    this->scale[1] = 1.0f;
    this->rotation = 0.0f;
    this->translation = Point(0, 0); 
    buildRotateAndScaleMatrix();
}

Shape::Shape(Point translation, float scale[2], float rotation)
{
    this->translation = translation;
    this->scale[0] = scale[0];
    this->scale[1] = scale[1];
    this->rotation = rotation;
    buildRotateAndScaleMatrix();
}

Shape::~Shape()
{
    //dtor
}

double Shape::toRadians(double degrees)
{
    return degrees * 0.017453292519943295;
}

void Shape::buildRotateAndScaleMatrix() {
    float scaleMatrix[3][3] = {
        {scale[0], 0.0f,  0.0f},
        {0.0f,  scale[1], 0.0f},
        {0.0f,  0.0f,  1.0f}
    };

    float rad = toRadians(rotation); 
    float cosR = std::cos(rad);
    float sinR = std::sin(rad);
    float rotationMatrix[3][3] = {
        {cosR, -sinR, 0.0f},
        {sinR,  cosR, 0.0f},
        {0.0f,  0.0f, 1.0f}
    };
    
    multiplyMatrix3x3(rotationMatrix, scaleMatrix, rotateAndScaleMatrix);
}

void Shape::setTranslation(Point translation)
{
    this->translation = translation;
}

void Shape::setRotation(float rotation)
{
    this->rotation = rotation;
    buildRotateAndScaleMatrix();
}

void Shape::setScale(float scale[2])
{
    this->scale[0] = scale[0];
    this->scale[1] = scale[1];
    buildRotateAndScaleMatrix();
}

Point Shape::getTranslation()
{
    return this->translation;
}

float Shape::getRotation()
{
    return this->rotation;
}

float* Shape::getScale()
{
    return this->scale;
}

void Shape::multiplyMatrix3x3(float A[3][3], float B[3][3], float result[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = 0.0f;
            for (int k = 0; k < 3; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/**
 * The first point of the list will be the used as pivot for the rotation
 */
std::list<Point> Shape::transform(std::list<Point>& points)
{
    std::list<Point> transformedPoints;

    if (points.empty()) {
        return transformedPoints;
    }

    Point basePoint = Point(points.front().getX(), points.front().getY());

    return transform(points, basePoint);
}

std::list<Point> Shape::transform(std::list<Point>& points, Point pivot)
{
    std::list<Point> transformedPoints;

    float translationMatrix[3][3] = {
        {1.0f, 0.0f, static_cast<float>(translation.getX())},
        {0.0f, 1.0f, static_cast<float>(translation.getY())},
        {0.0f, 0.0f, 1.0f} 
    };

    float transformMatrix[3][3];
    multiplyMatrix3x3(translationMatrix, rotateAndScaleMatrix, transformMatrix);
    
    for (Point p : points) {
        Point newP;

        int xOrigin = p.getX() - pivot.getX();
        int yOrigin = p.getY() - pivot.getY();

        float xRotatedAndScaled = rotateAndScaleMatrix[0][0] * xOrigin + rotateAndScaleMatrix[0][1] * yOrigin;
        float yRotatedAndScaled = rotateAndScaleMatrix[1][0] * xOrigin + rotateAndScaleMatrix[1][1] * yOrigin;

        int finalX = static_cast<int>(xRotatedAndScaled) + pivot.getX() + translation.getX();
        int finalY = static_cast<int>(yRotatedAndScaled) + pivot.getY() + translation.getY();

        newP.setX(finalX);
        newP.setY(finalY);

        transformedPoints.push_back(newP);
    }

    return transformedPoints;
}
