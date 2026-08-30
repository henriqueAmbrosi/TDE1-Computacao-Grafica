#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>
#include <list>
#include <memory>
#include "Point.h"
#include "Drawable.h"

class Events
{
public:
    Events();
    virtual ~Events();

    bool pollEvents();

private:
    void handleKeyDown(SDL_KeyboardEvent& keyEvent);
    void handleMouseButtonUp(SDL_MouseButtonEvent& mouseEvent);
    void handleMouseMotion(SDL_MouseMotionEvent& motionEvent);

    std::list<Point> tempPolygonPoints;
    std::list<std::shared_ptr<Drawable>> tempPolygonLines;

    void resetPolygonCreation();
    void finishPolygon();
};

#endif // EVENTS_H