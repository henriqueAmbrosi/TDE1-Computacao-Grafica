#ifndef SHAPE_H
#define SHAPE_H
#include "Point.h" // Modificado para aspas se estiver no mesmo diretório
#include <list>

class Shape
{
    public:
        Shape();
        Shape(Point translation, const float scale[2], float rotation);

        void setTranslation(Point translation);
        void setRotation(float rotation);
        void setScale(const float scale[2]);

        Point getTranslation();
        float getRotation();
        const float* getScale() const; // Retorna ponteiro constante para ler os valores

        // Abstract interface
        virtual void draw() = 0;

        virtual ~Shape();

    protected:
        // Mantido const para garantir segurança dos dados originais
        std::list<Point> transform(const std::list<Point>& points);

    private:
        float scale[2] = { 1.0f, 1.0f }; // Sintaxe corrigida para array
        float rotation = 0.0f;
        Point translation;

        double toRadians(double degrees);
        float rotateAndScaleMatrix[3][3];
        void buildRotateAndScaleMatrix();
        void multiplyMatrix3x3(const float A[3][3], const float B[3][3], float result[3][3]);
};

#endif // SHAPE_H