#include "ToolMenu.h"
#include "Context.h"
#include "Color.h"
#include "Rectangle.h"
#include "Point.h"

ToolMenu::ToolMenu()
{
}

ToolMenu::~ToolMenu()
{
}

void ToolMenu::draw()
{
    Context* ctx = Context::getInstance();

    if (!ctx->isMenuVisible()) {
        return;
    }

    SDL_Surface* surface = ctx->getWindowSurface();
    if (surface) {
        Color menuColor = Color(200, 200, 200);

        Point p1 = Point(0, 0);
        Point p2 = Point(surface->w, 40);
        Rectangle rect = Rectangle(p1, p2, menuColor);
        rect.draw();
    }
}