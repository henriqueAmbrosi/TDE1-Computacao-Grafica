#include "Events.h"
#include "Context.h"
#include "Point.h"
#include "Rectangle.h"
#include "Line.h"
#include "Polygon.h"
#include "Circle.h"
#include "FloodFill.h"
#include "Drawable.h"
#include <stdio.h>
#include <memory>

Events::Events()
{
}

Events::~Events()
{
}

bool Events::pollEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }

        switch (event.type)
        {
            case SDL_KEYDOWN:
                handleKeyDown(event.key);
                break;

            case SDL_MOUSEMOTION:
                handleMouseMotion(event.motion);
                break;

            case SDL_MOUSEBUTTONUP:
                handleMouseButtonUp(event.button);
                break;

            default:
                break;
        }
    }

    return true;
}

void Events::handleKeyDown(SDL_KeyboardEvent& keyEvent)
{
    switch (keyEvent.keysym.sym)
    {
        case SDLK_DELETE: {
            std::shared_ptr<Shape> selectedShape = Context::getInstance()->getSelectedFigure();
            if (selectedShape) {
                Context::getInstance()->removeDrawable(selectedShape);
                Context::getInstance()->setSelectedFigure(nullptr);
                printf("Selected shape deleted.\n");
            }
            break;
        }
        case SDLK_r:
            Context::getInstance()->setSelectedTool(Tool::RECTANGLE);
            printf("Tool switched to: Rectangle\n");
            break;
        case SDLK_l:
            Context::getInstance()->setSelectedTool(Tool::LINE);
            printf("Tool switched to: Line\n");
            break;
        case SDLK_c:
            Context::getInstance()->setSelectedTool(Tool::CIRCLE);
            printf("Tool switched to: Circle\n");
            break;
        case SDLK_u:
            Context::getInstance()->setSelectedTool(Tool::CURVE);
            printf("Tool switched to: Curve\n");
            break;
        case SDLK_p:
            Context::getInstance()->setSelectedTool(Tool::POLYGON);
            printf("Tool switched to: Polygon\n");
            break;
        case SDLK_s:
            Context::getInstance()->setSelectedTool(Tool::SELECT);
            printf("Tool switched to: Select\n");
            break;
        case SDLK_f:
            Context::getInstance()->setSelectedTool(Tool::PAINT);
            printf("Tool switched to: Paint (Flood Fill)\n");
            break;
        default:
            break;
    }
}

void Events::handleMouseMotion(SDL_MouseMotionEvent& motionEvent)
{

}

void Events::handleMouseButtonUp(SDL_MouseButtonEvent& mouseEvent)
{
    if (mouseEvent.button == SDL_BUTTON_LEFT)
    {
        int currentX = mouseEvent.x;
        int currentY = mouseEvent.y;
        Point clickPoint(currentX, currentY);

        Color currentColor = Context::getInstance()->getSelectedColor();
        Tool activeTool = Context::getInstance()->getSelectedTool();

        switch (activeTool)
        {
            case Tool::RECTANGLE: {
                Point end(currentX + 50, currentY + 50);
                Context::getInstance()->addDrawable(
                    std::make_shared<Rectangle>(clickPoint, end, currentColor, 1)
                );
                break;
            }
            case Tool::LINE: {
                Point end(currentX + 50, currentY + 50);
                Context::getInstance()->addDrawable(
                    std::make_shared<Line>(clickPoint, end, currentColor, 1)
                );
                break;
            }
            case Tool::CIRCLE: {
                Context::getInstance()->addDrawable(
                    std::make_shared<Circle>(clickPoint, 25, currentColor)
                );
                break;
            }
            case Tool::CURVE: {
                break;
            }
            case Tool::POLYGON: {
                break;
            }
            case Tool::SELECT: {
                bool found = false;

                for (const std::shared_ptr<Drawable>& drawable : Context::getInstance()->getDrawables()) {
                    // Verifica se Drawable é Shape
                    std::shared_ptr<Shape> shape = std::dynamic_pointer_cast<Shape>(drawable);

                    if (shape && shape->isInBoundary(clickPoint)) {
                        Context::getInstance()->setSelectedFigure(shape);
                        printf("Shape selected!\n");
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    Context::getInstance()->setSelectedFigure(nullptr);
                    printf("Selection cleared.\n");
                }
                break;
            }
            case Tool::PAINT: {
                auto floodFill = std::make_shared<FloodFill>(clickPoint, currentColor);
                Context::getInstance()->addDrawable(floodFill);
                printf("Area filled at (%d, %d)\n", currentX, currentY);
                break;
            }
        }
    }
}