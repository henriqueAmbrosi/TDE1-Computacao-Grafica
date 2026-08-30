#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <unistd.h>

#include "Context.h"
#include "Events.h"

SDL_Window* pWindow = nullptr;
SDL_Surface* window_surface = nullptr;

void display()
{
    for (const auto& figure : Context::getInstance()->getFigures()) {
        figure->draw();
    }

    auto selectedShape = Context::getInstance()->getSelectedFigure();
    if (selectedShape) {
        selectedShape->drawBoundary(Color(0, 0, 255));
    }
}

void clear() 
{
    SDL_Surface* surface = Context::getInstance()->getWindowSurface();
    if (surface) {
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
    }
}

int main(int argc, char* args[])
{
    bool running = true;
    Events eventHandler;

    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
    {
        pWindow = SDL_CreateWindow("SDL_Classes",
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   640, 480,
                                   SDL_WINDOW_SHOWN);

        window_surface = SDL_GetWindowSurface(pWindow);
        Context::getInstance()->setWindowSurface(window_surface);
    }
    else {
        return 1;
    }

    while (running)
    {
        clear();
        display();

        usleep(100000);

        running = eventHandler.pollEvents();

        SDL_UpdateWindowSurface(pWindow);
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}