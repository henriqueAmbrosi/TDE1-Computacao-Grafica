#ifndef CONTEXT_H
#define CONTEXT_H

#include <SDL2/SDL.h>
#include "Color.h"
#include "Shape.h"
#include <list>
#include <memory>

enum class Tool {
    RECTANGLE,
    LINE,
    CIRCLE,
    CURVE,
    POLYGON,
    SELECT,
    PAINT,
};

class Context
{
public:
    static Context* getInstance();

    Context(Context&) = delete;
    Context& operator=(Context&) = delete;

    void setWindowSurface(SDL_Surface* window_surface);
    void setRenderer(SDL_Renderer* pRenderer);
    SDL_Surface* getWindowSurface();
    SDL_Renderer* getRenderer();

    Color getSelectedColor();
    void setSelectedColor(Color& color);

    Tool getSelectedTool();
    void setSelectedTool(Tool tool);

    std::shared_ptr<Shape> getSelectedFigure();
    void setSelectedFigure(std::shared_ptr<Shape> figure);

    std::list<std::shared_ptr<Shape>>& getFigures();
    void addFigure(std::shared_ptr<Shape> figure);
    void removeFigure(std::shared_ptr<Shape> figure);

    bool isMenuVisible();
    void setMenuVisible(bool visible);

private:
    Context();
    ~Context();

    static Context* _instance;

    SDL_Surface* _window_surface;
    SDL_Renderer* _pRenderer;
    Color selectedColor;
    Tool selectedTool;
    std::shared_ptr<Shape> selectedFigure;
    std::list<std::shared_ptr<Shape>> figures;
    bool menuVisible;
};

#endif // CONTEXT_H