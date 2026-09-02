#include "ToolMenu.h"
#include "Context.h"
#include "Color.h"
#include "Rectangle.h"
#include "Point.h"
#include "Paint.h"
#include "Icon.h"

struct ToolMenuItem {
    Tool tool;
    IconType iconType;
};

ToolMenuItem menuItems[] = {
    { Tool::RECTANGLE, IconType::RECTANGLE },
    { Tool::LINE, IconType::LINE },
    { Tool::CIRCLE, IconType::CIRCLE },
    { Tool::CURVE, IconType::CURVE },
    { Tool::POLYGON, IconType::POLYGON },
    { Tool::SELECT, IconType::SELECT },
    { Tool::PAINT, IconType::PAINT }
};

Color btnSelectedColor = Color(60, 132, 214);
Color btnColor = Color(160, 160, 160);

ToolMenu::ToolMenu()
{
    Point p1 = Point(0, 0);
    Point p2 = Point(640, 40);
    Color menuColor = Color(200, 200, 200);
    this->container = Rectangle(p1, p2, menuColor);    

    Color iconColor = Color(10, 10, 10);
    int startX = 10;
    int btnWidth = 35;
    int btnHeight = 30;
    int startY = 5;
    int gap = 10;
    int i = 0;

    for (const auto& item : menuItems) {

        int x1 = startX + i * (btnWidth + gap);
        int y1 = startY;
        int x2 = x1 + btnWidth;
        int y2 = y1 + btnHeight;

        Icon icon(item.iconType, iconColor);
        Tool targetTool = item.tool;

        Button button(Point(x1, y1), Point(x2, y2), btnColor, icon, [targetTool]() {
            Context::getInstance()->setSelectedTool(targetTool);
        });

        this->buttons.push_back(button);
        i++;
    }
}

ToolMenu::~ToolMenu()
{
}


void ToolMenu::onClick(Point clickedPoint){
    for (Button& button : this->buttons) {
        if (button.clicked(clickedPoint)) {
            break;
        }
    }
}

bool ToolMenu::isInBoundary(Point clickedPoint){
    return this->container.isInBoundary(clickedPoint);
}

void ToolMenu::draw()
{   
    this->container.draw();
    Paint paint;
    paint.forceFill(Point(1, 20), this->container.getColor(),this->container.getColor());

    for (Button& button : this->buttons) {
        button.draw();
    }

}