#ifndef BUTTON_H
#define BUTTON_H

#include "Drawable.h"
#include "Rectangle.h"
#include "Point.h"
#include "Color.h"
#include "Icon.h"
#include <functional>

class Button : public Drawable
{
public:
    Button();
    Button(Point start, Point end, Color color, std::function<void()> onClick = nullptr);
    Button(Point start, Point end, Color color, Icon icon, std::function<void()> onClick = nullptr);
    virtual ~Button();

    void draw() override;
    bool clicked(Point clickPoint);

    void setOnClick(std::function<void()> onClick);
    void setIcon(Icon icon);
    void setColor(Color color);

private:
    bool isClicked(Point clickPoint);
    Rectangle container;
    Icon icon;
    bool hasIcon;
    Point start;
    Point end;
    std::function<void()> onClick;
};

#endif // BUTTON_H