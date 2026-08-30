#ifndef CONTEXT_H
#define CONTEXT_H

#include <SDL2/SDL.h>
#include <list>
#include <memory>

#include "Color.h"
#include "Drawable.h"
#include "Shape.h"

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

    std::list<std::shared_ptr<Drawable>>& getDrawables();
    void addDrawable(std::shared_ptr<Drawable> drawable);
    void removeDrawable(std::shared_ptr<Drawable> drawable);

    bool isMenuVisible();
    void setMenuVisible(bool menuVisible);

private:
    Context();
    ~Context();

    static Context* instance;

    SDL_Surface* window_surface;
    SDL_Renderer* pRenderer;
    Color selectedColor;
    Tool selectedTool;
    std::shared_ptr<Shape> selectedFigure;
    std::list<std::shared_ptr<Drawable>> drawables;
    bool menuVisible;
};

#endif // CONTEXT_H