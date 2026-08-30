#include "Events.h"
#include "Context.h"
#include "Point.h"
#include "Rectangle.h"
#include "Line.h"
#include "Polygon.h"
#include "Circle.h"
#include "Paint.h"
#include "Drawable.h"
#include "ToolMenu.h"
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

void Events::resetPolygonCreation()
{
    // Remove do Context todas as linhas provisórias desenhadas
    for (const auto& line : tempPolygonLines) {
        Context::getInstance()->removeDrawable(line);
    }
    tempPolygonLines.clear();
    tempPolygonPoints.clear();
}

void Events::finishPolygon()
{
    if (tempPolygonPoints.size() >= 3) {
        Color currentColor = Context::getInstance()->getSelectedColor();

        auto polygon = std::make_shared<Polygon>(tempPolygonPoints, currentColor);

        resetPolygonCreation();

        Context::getInstance()->addDrawable(polygon);
        printf("Polygon completed with success!\n");
    } else {
        printf("Polygon requires at least 3 points. Operation canceled.\n");
        resetPolygonCreation();
    }
}

void Events::handleKeyDown(SDL_KeyboardEvent& keyEvent)
{
    if (keyEvent.keysym.sym == SDLK_ESCAPE) {
        if (!tempPolygonPoints.empty()) {
            resetPolygonCreation();
            printf("Polygon creation canceled.\n");
        }
        return;
    }

    if (keyEvent.keysym.sym == SDLK_RETURN || keyEvent.keysym.sym == SDLK_KP_ENTER) {
        if (Context::getInstance()->getSelectedTool() == Tool::POLYGON) {
            finishPolygon();
        }
        return;
    }

    // Troca de ferramenta (reseta construção pendente)
    Tool previousTool = Context::getInstance()->getSelectedTool();

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

    // Se mudou de ferramenta enquanto desenhava um polígono, limpa o rascunho
    if (previousTool != Context::getInstance()->getSelectedTool() && !tempPolygonPoints.empty()) {
        resetPolygonCreation();
    }
}

void Events::handleMouseMotion(SDL_MouseMotionEvent& motionEvent)
{

}

void Events::handleMouseButtonUp(SDL_MouseButtonEvent& mouseEvent)
{
    Tool activeTool = Context::getInstance()->getSelectedTool();
    Color currentColor = Context::getInstance()->getSelectedColor();
    Point clickPoint(mouseEvent.x, mouseEvent.y);
    ToolMenu toolMenu = Context::getInstance()->getToolMenu();

    if(toolMenu.isInBoundary(clickPoint)){
        toolMenu.onClick(clickPoint);
        return;
    }
    
    // Clique com o Botão Direito finaliza o Polígono
    if (mouseEvent.button == SDL_BUTTON_RIGHT) {
        if (activeTool == Tool::POLYGON && !tempPolygonPoints.empty()) {
            finishPolygon();
        }
        return;
    }

    if (mouseEvent.button == SDL_BUTTON_LEFT) {
        switch (activeTool)
        {
            case Tool::RECTANGLE: {
                Point end(clickPoint.getX() + 50, clickPoint.getY() + 50);
                Context::getInstance()->addDrawable(
                    std::make_shared<Rectangle>(clickPoint, end, currentColor, 1)
                );
                break;
            }
            case Tool::LINE: {
                Point end(clickPoint.getX() + 50, clickPoint.getY() + 50);
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
                if (!tempPolygonPoints.empty()) {
                    // Desenha uma linha provisória ligando o último ponto ao novo clique
                    Point lastPoint = tempPolygonPoints.back();
                    auto linePreview = std::make_shared<Line>(lastPoint, clickPoint, currentColor, 1);
                    
                    Context::getInstance()->addDrawable(linePreview);
                    tempPolygonLines.push_back(linePreview);
                }

                tempPolygonPoints.push_back(clickPoint);
                printf("Polygon point added: (%d, %d)\n", clickPoint.getX(), clickPoint.getY());
                break;
            }
            case Tool::SELECT: {
                bool found = false;

                for (const std::shared_ptr<Drawable>& drawable : Context::getInstance()->getDrawables()) {
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
                auto floodFill = std::make_shared<Paint>(clickPoint, currentColor);
                Context::getInstance()->addDrawable(floodFill);
                printf("Area filled at (%d, %d)\n", clickPoint.getX(), clickPoint.getY());
                break;
            }
        }
    }
}