#ifndef TOOLMENU_H
#define TOOLMENU_H

#include "Point.h"
#include "Rectangle.h"
#include "Button.h"
#include "Line.h"
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
    std::list<Button> toolButtons;
    Line separator;
    std::list<Button> colorButtons;
};

#endif // TOOLMENU_H