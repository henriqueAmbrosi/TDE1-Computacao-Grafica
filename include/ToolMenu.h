#ifndef TOOLMENU_H
#define TOOLMENU_H

#include "Point.h"
#include "Rectangle.h"

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
};

#endif // TOOLMENU_H