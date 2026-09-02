#include "Button.h"
#include "Paint.h"
#include "stdio.h"

Button::Button()
{
    this->onClick = nullptr;
    this->hasIcon = false;
}

Button::Button(Point start, Point end, Color color, std::function<void()> onClick)
{
    this->start = start;
    this->end = end;
    this->container = Rectangle(start, end, color);
    this->onClick = onClick;
    this->hasIcon = false;
}

Button::Button(Point start, Point end, Color color, Icon icon, std::function<void()> onClick)
{
    this->start = start;
    this->end = end;
    this->container = Rectangle(start, end, color);
    this->icon = icon;
    this->onClick = onClick;
    this->hasIcon = true;
}

Button::~Button()
{
}

void Button::draw()
{
    this->container.draw();
    Color fillColor = this->container.getColor();
    Point btnStart = this->container.getStartPoint();
    Point fillSeed = Point(btnStart.getX() + 1, btnStart.getY() + 1);
    Paint fillButton = Paint();
    fillButton.forceFill(fillSeed, fillColor, fillColor);

    if (this->hasIcon) {
        Point center(
            (this->start.getX() + this->end.getX()) / 2,
            (this->start.getY() + this->end.getY()) / 2
        );
        this->icon.drawAt(center);
    }
}

bool Button::clicked(Point clickPoint)
{
    if (this->isClicked(clickPoint)) {
        if (this->onClick) {
            this->onClick();
            return true;
        }
    }
    return false;
}

bool Button::isClicked(Point clickPoint)
{
    return this->container.isInBoundary(clickPoint);
}

void Button::setOnClick(std::function<void()> onClick)
{
    this->onClick = onClick;
}

void Button::setIcon(Icon icon)
{
    this->icon = icon;
    this->hasIcon = true;
}

void Button::setColor(Color c)
{
    this->container.setColor(c);
}