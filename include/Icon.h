#ifndef ICON_H
#define ICON_H

#include "Drawable.h"
#include "Point.h"
#include "Color.h"

enum class IconType {
    RECTANGLE,
    LINE,
    CIRCLE,
    CURVE,
    POLYGON,
    SELECT,
    PAINT
};

class Icon : public Drawable
{
public:
    Icon();
    Icon(IconType type, Color color);
    Icon(IconType type, Color color, Point center);
    virtual ~Icon();

    void draw() override;
    void drawAt(Point center);

    IconType getType();
    void setType(IconType type);
    void setColor(Color color);

private:
    IconType type;
    Color color;
    Point center;

    void drawRectangleIcon(Point center);
    void drawLineIcon(Point center);
    void drawCircleIcon(Point center);
    void drawCurveIcon(Point center);
    void drawPolygonIcon(Point center);
    void drawSelectIcon(Point center);
    void drawPaintIcon(Point center);
};

#endif // ICON_H