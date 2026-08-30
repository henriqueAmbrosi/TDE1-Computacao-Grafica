#include "Paint.h"
#include "Context.h"
#include <stack>

Paint::Paint() {
}

Paint::Paint(Point p, Color c) {
    this->fillColor = c;
    this->fillPoint = p;
}


Paint::~Paint() {
}

Uint32 Paint::getPixel(int x, int y) {
    SDL_Surface* window_surface = Context::getInstance()->getWindowSurface();
    if (!window_surface) return 0;

    if (x < 0 || x >= window_surface->w || y < 0 || y >= window_surface->h) {
        return 0;
    }

    int bpp = window_surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)window_surface->pixels + y * window_surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32*)p;
        default:
            return 0;
    }
}

void Paint::setPixel(int x, int y, Uint32 pixelColor) {
    SDL_Surface* window_surface = Context::getInstance()->getWindowSurface();
    if (!window_surface) return;

    if (x < 0 || x >= window_surface->w || y < 0 || y >= window_surface->h) {
        return;
    }

    unsigned int* pixels = (unsigned int*)window_surface->pixels;
    pixels[x + y * window_surface->w] = pixelColor;
}

void Paint::setPixel(int x, int y, Color color) {
    this->setPixel(x, y, color.getR(), color.getG(), color.getB(), 255);
}

void Paint::setPixel(int x, int y, int r, int g, int b, int a) {
    SDL_Surface* window_surface = Context::getInstance()->getWindowSurface();
    if (!window_surface) return;

    if (x < 0 || x >= window_surface->w || y < 0 || y >= window_surface->h) {
        return;
    }

    unsigned int* pixels = (unsigned int*)window_surface->pixels;
    pixels[x + y * window_surface->w] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

void Paint::fill(Point point, Color fillColor) {
    this->fill(point.getX(), point.getY(), fillColor);
}

void Paint::forceFill(Point point, Color fillColor, Color limitColor){
    int startX = point.getX();
    int startY = point.getY();
    this->fillColor = fillColor;
    this->fillPoint = Point(startX, startY);
    
    SDL_Surface* surface = Context::getInstance()->getWindowSurface();
    if (!surface) return;

    int width = surface->w;
    int height = surface->h;

    if (startX < 0 || startX >= width || startY < 0 || startY >= height) {
        return;
    }

    Uint32 fillPixel = SDL_MapRGB(surface->format, fillColor.getR(), fillColor.getG(), fillColor.getB());
    Uint32 stopColor = limitColor.getColor();

    if (fillPixel != stopColor) {
        return;
    }

    std::stack<Point> st;
    st.push(Point(startX, startY));

    while (!st.empty()) {
        Point p = st.top();
        st.pop();

        int x = p.getX();
        int y = p.getY();

        if (x < 0 || x >= width || y < 0 || y >= height) {
            continue;
        }

        if (this->getPixel(x, y) != stopColor) {
            this->setPixel(x, y, fillPixel);

            st.push(Point(x + 1, y));
            st.push(Point(x - 1, y));
            st.push(Point(x, y + 1));
            st.push(Point(x, y - 1));
        }
    }
}

void Paint::draw(){
    fill(this->fillPoint, this->fillColor);
}

void Paint::fill(int startX, int startY, Color fillColor) {
    this->fillColor = fillColor;
    this->fillPoint = Point(startX, startY);
    
    SDL_Surface* surface = Context::getInstance()->getWindowSurface();
    if (!surface) return;

    int width = surface->w;
    int height = surface->h;

    if (startX < 0 || startX >= width || startY < 0 || startY >= height) {
        return;
    }

    Uint32 fillPixel = SDL_MapRGB(surface->format, fillColor.getR(), fillColor.getG(), fillColor.getB());
    Uint32 defaultPixel = this->getPixel(startX, startY);

    if (fillPixel == defaultPixel) {
        return;
    }

    std::stack<Point> st;
    st.push(Point(startX, startY));

    while (!st.empty()) {
        Point p = st.top();
        st.pop();

        int x = p.getX();
        int y = p.getY();

        if (x < 0 || x >= width || y < 0 || y >= height) {
            continue;
        }

        if (this->getPixel(x, y) == defaultPixel) {
            this->setPixel(x, y, fillPixel);

            st.push(Point(x + 1, y));
            st.push(Point(x - 1, y));
            st.push(Point(x, y + 1));
            st.push(Point(x, y - 1));
        }
    }
}