#ifndef CONTEXT_H
#define CONTEXT_H

#include<SDL2/SDL.h>
#include <Color.h>
#include <Shape.h>
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
        static Context * getInstance();
        void setWindowSurface(SDL_Surface * window_surface);
        void setRenderer(SDL_Renderer * pRenderer);
        SDL_Surface * getWindowSurface();
        SDL_Renderer * getRenderer();

    protected:
        Context();
    private:
        virtual ~Context();
        static Context * _instance;
        SDL_Surface * _window_surface;
        SDL_Renderer * _pRenderer;

        Color selectedColor;
        Tool selectedTool;
        std::shared_ptr<Shape> selectedFigure;
        std::list<std::shared_ptr<Shape>> figures;

};

#endif // CONTEXT_H
