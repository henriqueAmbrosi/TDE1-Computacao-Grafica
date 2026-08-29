#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include <Context.h>
#include <Color.h>
#include <Point.h>
#include <Line.h>
#include <Shape.h>
#include <Rectangle.h>
#include <list>
#include <memory>
#include<Polygon.h>

// SDL stuff
SDL_Window* pWindow = nullptr;
SDL_Renderer* pRenderer = nullptr;
SDL_Surface* window_surface = nullptr;

std::list<std::shared_ptr<Shape>> figures;

void display()
{
    for (const auto& f : figures) {
        f->draw();
    }
}

// Optimized screen clearing
void clear() 
{
    SDL_Surface* surface = Context::getInstance()->getWindowSurface();
    if (surface) {
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
    }
}

int main(int argc, char* args[])
{
    int currentX, currentY;
    Color color = Color(255, 0, 0);
    Tool activeTool = Tool::RECTANGLE;
    bool running = true;

    SDL_Event event;

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

        // Process all pending events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            // 1. Keyboard Event Handling
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_r:
                        activeTool = Tool::RECTANGLE;
                        printf("Tool switched to: Rectangle\n");
                        break;
                    case SDLK_l:
                        activeTool = Tool::LINE;
                        printf("Tool switched to: Line\n");
                        break;
                    case SDLK_c:
                        activeTool = Tool::CIRCLE;
                        printf("Tool switched to: Circle\n");
                        break;
                    case SDLK_u:
                        activeTool = Tool::CURVE;
                        printf("Tool switched to: Curve\n");
                        break;
                    case SDLK_p:
                        activeTool = Tool::POLYGON;
                        printf("Tool switched to: Polygon\n");
                        break;
                    case SDLK_s:
                        activeTool = Tool::SELECT;
                        printf("Tool switched to: Select\n");
                        break;
                    default:
                        break;
                }
            }

            // 2. Mouse Motion
            if (event.type == SDL_MOUSEMOTION)
            {
                // Hover or drag preview logic goes here
            }

            // 3. Mouse Button Release
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    currentX = event.button.x;
                    currentY = event.button.y;
                    Point start = Point(currentX, currentY);

                    switch (activeTool)
                    {
                        case Tool::RECTANGLE: {
                            Point end = Point(currentX + 50, currentY + 50);
                            figures.push_back(std::make_shared<Rectangle>(start, end, color, 1));
                            break;
                        }
                        case Tool::LINE: {
                            Point end = Point(currentX + 50, currentY + 50);
                            figures.push_back(std::make_shared<Line>(start, end, color, 1));
                            break;
                        }
                        case Tool::CIRCLE: {
                            break;
                        }
                        case Tool::CURVE: {
                            break;
                        }
                        case Tool::POLYGON: {
                            break;
                        }
                        case Tool::SELECT: {
                            break;
                        }
                    }
                }
            }
        } // End of SDL_PollEvent loop

        SDL_UpdateWindowSurface(pWindow);
    }

    // Clean up
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}