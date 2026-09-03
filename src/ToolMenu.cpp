#include "ToolMenu.h"
#include "Context.h"
#include "Color.h"
#include "Rectangle.h"
#include "Point.h"
#include "Paint.h"
#include "Icon.h"
#include "Line.h"
#include "ImageSaver.h"

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

Color colorOptions[] = {
    Color(0, 0, 0),       // Preto
    Color(255, 255, 255), // Branco
    Color(128, 128, 128), // Cinza Médio
    Color(139, 69, 19),   // Marrom
    Color(230, 40, 40),   // Vermelho
    Color(255, 128, 0),   // Laranja
    Color(255, 215, 0),   // Amarelo
    Color(46, 204, 113),  // Verde
    Color(52, 152, 219),  // Azul Céu
    Color(15, 82, 186),   // Azul Marinho
    Color(155, 89, 182),  // Roxo
    Color(255, 105, 180)  // Rosa
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
    int startX = 7;
    int btnWidth = 24;
    int btnHeight = 24;
    int startY = 8;
    int gap = 7;
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

        this->toolButtons.push_back(button);
        i++;
    }

    Icon saveIcon(IconType::SAVE, Color(10, 10, 10));

    int saveX1 = startX + i * (btnWidth + gap);
    int saveY1 = startY;
    int saveX2 = saveX1 + btnWidth;
    int saveY2 = saveY1 + btnHeight;

    Button saveButton(Point(saveX1, saveY1), Point(saveX2, saveY2), btnColor, saveIcon, []() {
        Context* context = Context::getInstance();
        SDL_Surface* surface = context->getWindowSurface();

        if (!surface) return;

        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));

        for (std::shared_ptr<Drawable>& figure : context->getDrawables()) {
            if (figure) {
                figure->draw();
            }
        }

        ImageSaver::saveCanvas(surface);
    });

    this->saveButton = saveButton;
    
    i++;
    int xToolBtnEnd = startX + i * (btnWidth + gap);
    p1 = Point(xToolBtnEnd, 5);
    p2 = Point(xToolBtnEnd, 35);
    Color separatorColor = Color(180, 180, 180);
    Line separator = Line(p1, p2, separatorColor);
    this->separator = separator;

    int colorButtonsXStart = xToolBtnEnd + 1 + gap;
    i = 0;
    for (Color color : colorOptions) {
        int x1 = colorButtonsXStart + i * (btnWidth + gap);
        int y1 = startY;
        int x2 = x1 + btnWidth;
        int y2 = y1 + btnHeight;

        Icon icon = Icon(IconType::COLOR, color);

        Button button(Point(x1, y1), Point(x2, y2), btnColor, icon, [color]() {
            Color tempColor = color;
            Context::getInstance()->setSelectedColor(tempColor);
        });

        this->colorButtons.push_back(button);
        i++;
    }
}

ToolMenu::~ToolMenu()
{
}

void ToolMenu::onClick(Point clickedPoint){
    for (Button& button : this->toolButtons) {
        if (button.clicked(clickedPoint)) {
            break;
        }
    }

    this->saveButton.clicked(clickedPoint);

    for (Button& button : this->colorButtons) {
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
    paint.forceFill(Point(1, 20), this->container.getColor(), this->container.getColor());

    Tool currentTool = Context::getInstance()->getSelectedTool();
    Color currentColor = Context::getInstance()->getSelectedColor();

    int i = 0;
    for (Button& button : this->toolButtons) {
        if (menuItems[i].tool == currentTool) {
            button.setColor(btnSelectedColor);
        } else {
            button.setColor(btnColor);
        }
        button.draw();
        i++;
    }

    this->saveButton.draw();
    this->separator.draw();

    i = 0;
    for (Button& button : this->colorButtons) {
        if (colorOptions[i].getColor() == currentColor.getColor()) {
            button.setColor(btnSelectedColor);
        } else {
            button.setColor(btnColor);
        }
        button.draw();
        i++;
    }
}