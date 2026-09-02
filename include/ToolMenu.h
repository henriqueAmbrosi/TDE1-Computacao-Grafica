#ifndef TOOLMENU_H
#define TOOLMENU_H

#include "Point.h"
#include "Rectangle.h"
#include "Button.h"
#include <list>

class ToolMenu
{
public:
    ToolMenu();
    ~ToolMenu();

    void draw();
    void onClick(Point clickedPoint);
    bool isInBoundary(Point clickedPoint);
private:
    Rectangle container;
    std::list<Button> buttons;
};

#endif // TOOLMENU_H