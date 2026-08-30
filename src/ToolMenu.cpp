#include "ToolMenu.h"
#include "Context.h"
#include "Color.h"
#include "Rectangle.h"
#include "Point.h"
#include "Paint.h"
#include "Point.h"

ToolMenu::ToolMenu()
{
    Point p1 = Point(0, 0);
    Point p2 = Point(640, 40);
    Color menuColor = Color(200, 200, 200);
    this->container = Rectangle(p1, p2, menuColor);    
}

ToolMenu::~ToolMenu()
{
}


void ToolMenu::onClick(Point clickedPoint){

}

bool ToolMenu::isInBoundary(Point clickedPoint){
    return this->container.isInBoundary(clickedPoint);
}

void ToolMenu::draw()
{   
    this->container.draw();
    Paint paint;
    paint.forceFill(Point(1, 20), this->container.getColor(),this->container.getColor());
}