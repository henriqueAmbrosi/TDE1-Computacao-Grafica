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
#include "BezierCurve.h"
#include <stdio.h>
#include <memory>
#include <algorithm>

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

            case SDL_MOUSEBUTTONDOWN:
                handleMouseButtonDown(event.button);
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
    if (Context::getInstance()->isResizing()) {
        Point currentPoint = Point(motionEvent.x, motionEvent.y);
        Context* ctx = Context::getInstance();
        std::shared_ptr<Shape> shape = ctx->getSelectedFigure();
        Point init = ctx->getInitalResizePoint();
        int dx = currentPoint.getX() - init.getX();
        int dy = currentPoint.getY() - init.getY();
    
        switch (ctx->getResizeAnchor()) {
            case Anchor::TOP_LEFT:
                dx = dx;
                dy = dy;
                break;
            case Anchor::TOP_RIGHT:
                dy = dy;
                break;
            case Anchor::BOTTOM_LEFT:
                dx = -dx;
                break;
            case Anchor::BOTTOM_RIGHT:
                dx = std::abs(dx);
            default:
                break;
        }
    
        float startSx = ctx->getResizeStartScaleX();
        float startSy = ctx->getResizeStartScaleY();
        float refW = std::max(ctx->getResizeRefWidth(), 1.0f);
        float refH = std::max(ctx->getResizeRefHeight(), 1.0f);
    
        float kx = 1.0f + (float) (dx) / refW;
        float ky = 1.0f + (float) (dy) / refH;
    
        if (std::dynamic_pointer_cast<Circle>(shape)) {
            float k = 0.5f * (kx + ky);
            kx = k;
            ky = k;
        }
    
        float scaleX = startSx * kx;
        float scaleY = startSy * ky;
    
        const float minScale = 0.05f;
        if (scaleX < minScale) {
            scaleX = minScale;
        }
        if (scaleY < minScale) {
            scaleY = minScale;
        }
    
        shape->setScale(scaleX, scaleY);    }
}

void Events::finishCurve() {
    Color currentColor = Context::getInstance()->getSelectedColor();

    auto curve = std::make_shared<BezierCurve>(tempPolygonPoints, currentColor);

    resetPolygonCreation();

    Context::getInstance()->addDrawable(curve);
    printf("Curve completed with success!\n");
}

void Events::handleMouseButtonDown(SDL_MouseButtonEvent& mouseEvent)
{
    Tool activeTool = Context::getInstance()->getSelectedTool();
    Point clickPoint(mouseEvent.x, mouseEvent.y);

    if (mouseEvent.button == SDL_BUTTON_LEFT) {
        if (activeTool == Tool::SELECT) {
                std::shared_ptr<Shape> selected = Context::getInstance()->getSelectedFigure();
                if (!selected) {
                    return;
                }

                Anchor anchor = selected->inAnchors(clickPoint);
                if (anchor == Anchor::NONE) {
                    return;
                }

                float localW = 1.0f;
                float localH = 1.0f;
                selected->getLocalSize(localW, localH);

                Context::getInstance()->setIsResizing(true);
                Context::getInstance()->setInitialResizePoint(clickPoint);
                Context::getInstance()->setResizeAnchor(anchor);
                Context::getInstance()->setResizeStartScale(selected->getScale()[0], selected->getScale()[1]);
                Context::getInstance()->setResizeRefSize(localW, localH);
                printf("Clicou na âncora\n");
        }
    }
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
                if (!tempPolygonPoints.empty()) {
                    // Desenha uma linha provisória ligando o último ponto ao novo clique
                    Point lastPoint = tempPolygonPoints.back();
                    auto linePreview = std::make_shared<Line>(lastPoint, clickPoint, currentColor, 1);
                    
                    Context::getInstance()->addDrawable(linePreview);
                    tempPolygonLines.push_back(linePreview);
                }

                tempPolygonPoints.push_back(clickPoint);
                if (tempPolygonPoints.size() == 4) {
                    finishCurve();
                }
                printf("Curve point added: (%d, %d)\n", clickPoint.getX(), clickPoint.getY());
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
                if (Context::getInstance()->isResizing()) {
                    Context::getInstance()->setIsResizing(false);
                    break;
                }

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