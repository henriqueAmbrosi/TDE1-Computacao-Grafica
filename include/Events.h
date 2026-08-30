#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>

class Events
{
public:
    Events();
    virtual ~Events();

    // Polls all pending SDL events. Returns false when SDL_QUIT is triggered.
    bool pollEvents();

private:
    void handleKeyDown(const SDL_KeyboardEvent& keyEvent);
    void handleMouseButtonUp(const SDL_MouseButtonEvent& mouseEvent);
    void handleMouseMotion(const SDL_MouseMotionEvent& motionEvent);
};

#endif // EVENTS_H