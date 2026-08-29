#include "Shape.h"
#include <cmath>
#include <iostream>

Shape::Shape()
{
    this->scale[0] = 1.0f;
    this->scale[1] = 1.0f;
    this->rotation = 0.0f;
    buildRotateAndScaleMatrix();
}

Shape::Shape(Point translation, const float scale[2], float rotation)
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

void Shape::setScale(const float scale[2])
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

const float* Shape::getScale() const
{
    return this->scale;
}

void Shape::multiplyMatrix3x3(const float A[3][3], const float B[3][3], float result[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = 0.0f;
            for (int k = 0; k < 3; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

std::list<Point> Shape::transform(const std::list<Point>& points)
{
    std::list<Point> transformedPoints;

    // Correção: Acessando x e y de translation via métodos públicos e convertendo para float
    float translationMatrix[3][3] = {
        {1.0f, 0.0f, static_cast<float>(translation.getX())},
        {0.0f, 1.0f, static_cast<float>(translation.getY())},
        {0.0f, 0.0f, 1.0f} 
    };

    float transformMatrix[3][3];
    multiplyMatrix3x3(translationMatrix, rotateAndScaleMatrix, transformMatrix);

    // Nota: Removido o 'const' daqui temporariamente para corrigir o erro caso você não queira alterar o Point.h agora.
    // O ideal é manter const e aplicar o passo 2 abaixo!
    for (Point p : points) {
        Point newP;

        int nextX = transformMatrix[0][0] * p.getX() + transformMatrix[0][1] * p.getY() + transformMatrix[0][2] * 1.0f;
        int nextY = transformMatrix[1][0] * p.getX() + transformMatrix[1][1] * p.getY() + transformMatrix[1][2] * 1.0f;

        newP.setX(nextX);
        newP.setY(nextY);

        transformedPoints.push_back(newP);
    }

    return transformedPoints;
}
