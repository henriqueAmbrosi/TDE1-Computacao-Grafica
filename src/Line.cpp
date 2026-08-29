#include "Line.h"
#include "Context.h"
#include "Rectangle.h"
#include <cmath>
#include <algorithm>

Line::Line()
{
}

Line::~Line()
{
}

Line::Line(Point start, Point end, Color color) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->antialias = 0;
}

Line::Line(Point start, Point end, Color color, int antialias) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->antialias = antialias;
}

void Line::setPixel(int x, int y, Uint32 cor) {
    Color c = Color();
    Uint8 r = c.getColorComponent(cor, 'r');
    Uint8 g = c.getColorComponent(cor, 'g');
    Uint8 b = c.getColorComponent(cor, 'b');
    this->setPixel(x, y, r, g, b);
}

void Line::setPixel(int x, int y, Color color) {
    this->setPixel(x, y, color.getR(), color.getG(), color.getB());
}

void Line::setPixel(int x, int y, int r, int g, int b) {
    setPixel(x, y, r, g, b, 255);
}

void Line::setPixel(int x, int y, int r, int g, int b, int a) {
    SDL_Surface* window_surface = Context::getInstance()->getWindowSurface();
    if (!window_surface) return;

    if (x < 0 || x >= window_surface->w || y < 0 || y >= window_surface->h) {
        return;
    }

    unsigned int* pixels = (unsigned int*)window_surface->pixels;
    pixels[x + y * window_surface->w] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

void Line::draw() {
    std::list<Point> localPoints = { this->start, this->end };
    std::list<Point> transformedPoints = transform(localPoints, this->start);

    auto it = transformedPoints.begin();
    Point p1 = *it;
    std::advance(it, 1);
    Point p2 = *it;

    if (this->antialias) {
        this->drawWuLine(p1.getX(), p1.getY(), p2.getX(), p2.getY(), this->color);
    }
    else {
        this->bresenham(p1.getX(), p1.getY(), p2.getX(), p2.getY(),
                       this->color.getR(), this->color.getG(), this->color.getB());
    }
}

void Line::drawBoundary(Color color) {
    std::list<Point> localPoints = { this->start, this->end };
    std::list<Point> transformedPoints = transform(localPoints, this->start);

    auto it = transformedPoints.begin();
    Point p1 = *it;
    std::advance(it, 1);
    Point p2 = *it;

    auto drawAnchor = [&](Point pt) {
        Point aStart(pt.getX() - 4, pt.getY() - 4);
        Point aEnd(pt.getX() + 4, pt.getY() + 4);
        Rectangle anchor(aStart, aEnd, color);
        anchor.draw();
    };

    drawAnchor(p1);
    drawAnchor(p2);

    int minX = std::min(p1.getX(), p2.getX()) - 4;
    int maxX = std::max(p1.getX(), p2.getX()) + 4;
    int minY = std::min(p1.getY(), p2.getY()) - 4;
    int maxY = std::max(p1.getY(), p2.getY()) + 4;

    Rectangle box(Point(minX, minY), Point(maxX, maxY), color);
    box.draw();
}

bool Line::isInBoundary(Point point) {
    std::list<Point> localPoints = { this->start, this->end };
    std::list<Point> transformedPoints = transform(localPoints, this->start);

    auto it = transformedPoints.begin();
    Point p1 = *it;
    std::advance(it, 1);
    Point p2 = *it;

    double px = point.getX();
    double py = point.getY();
    double x1 = p1.getX();
    double y1 = p1.getY();
    double x2 = p2.getX();
    double y2 = p2.getY();

    double minX = std::min(x1, x2) - 5.0;
    double maxX = std::max(x1, x2) + 5.0;
    double minY = std::min(y1, y2) - 5.0;
    double maxY = std::max(y1, y2) + 5.0;

    if (px < minX || px > maxX || py < minY || py > maxY) {
        return false;
    }

    double l2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    if (l2 == 0.0) {
        double dist = std::sqrt((px - x1) * (px - x1) + (py - y1) * (py - y1));
        return dist <= 5.0;
    }

    double t = std::max(0.0, std::min(1.0, ((px - x1) * (x2 - x1) + (py - y1) * (y2 - y1)) / l2));
    double projX = x1 + t * (x2 - x1);
    double projY = y1 + t * (y2 - y1);

    double distance = std::sqrt((px - projX) * (px - projX) + (py - projY) * (py - projY));
    return distance <= 5.0; // Click tolerance threshold in pixels
}

Uint32 Line::getPixel(int x, int y) {
    SDL_Surface* window_surface = Context::getInstance()->getWindowSurface();
    if (!window_surface) return 0;

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

void Line::drawWuLine(int x0, int y0, int x1, int y1, Color color) {
    if (y0 > y1) {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }

    setPixel(x0, y0, color);

    int xDir = (x1 >= x0) ? 1 : -1;
    int deltaX = std::abs(x1 - x0);
    int deltaY = y1 - y0;

    if (deltaY == 0) {
        while (deltaX-- != 0) {
            x0 += xDir;
            setPixel(x0, y0, color);
        }
        return;
    }
    if (deltaX == 0) {
        do {
            y0++;
            setPixel(x0, y0, color);
        } while (--deltaY != 0);
        return;
    }
    if (deltaX == deltaY) {
        do {
            x0 += xDir;
            y0++;
            setPixel(x0, y0, color);
        } while (--deltaY != 0);
        return;
    }

    unsigned short errorAdj, errorAccaux, weighting;
    unsigned short errorAcc = 0;

    Uint32 rl = color.getR();
    Uint32 gl = color.getG();
    Uint32 bl = color.getB();
    double grayl = rl * 0.299 + gl * 0.587 + bl * 0.114;

    if (deltaY > deltaX) {
        errorAdj = ((unsigned long)deltaX << 16) / (unsigned long)deltaY;
        while (--deltaY) {
            errorAccaux = errorAcc;
            errorAcc += errorAdj;
            if (errorAcc <= errorAccaux) {
                x0 += xDir;
            }
            y0++;
            weighting = errorAcc >> 8;

            Uint32 clrBackGround = getPixel(x0, y0);
            Uint8 rb = color.getColorComponent(clrBackGround, 'r');
            Uint8 gb = color.getColorComponent(clrBackGround, 'g');
            Uint8 bb = color.getColorComponent(clrBackGround, 'b');
            double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

            Uint8 rr = (rb > rl ? ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (rb - rl) + rl)) : ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (rl - rb) + rb)));
            Uint8 gr = (gb > gl ? ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (gb - gl) + gl)) : ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (gl - gb) + gb)));
            Uint8 br = (bb > bl ? ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (bb - bl) + bl)) : ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (bl - bb) + bb)));
            setPixel(x0, y0, color.getColor(rr, gr, br));

            clrBackGround = getPixel(x0 + xDir, y0);
            rb = color.getColorComponent(clrBackGround, 'r');
            gb = color.getColorComponent(clrBackGround, 'g');
            bb = color.getColorComponent(clrBackGround, 'b');
            grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

            rr = (rb > rl ? ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (rb - rl) + rl)) : ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (rl - rb) + rb)));
            gr = (gb > gl ? ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (gb - gl) + gl)) : ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (gl - gb) + gb)));
            br = (bb > bl ? ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (bb - bl) + bl)) : ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (bl - bb) + bb)));
            setPixel(x0 + xDir, y0, color.RGB(rr, gr, br));
        }
        setPixel(x1, y1, color);
        return;
    }

    errorAdj = ((unsigned long)deltaY << 16) / (unsigned long)deltaX;
    while (--deltaX) {
        errorAccaux = errorAcc;
        errorAcc += errorAdj;
        if (errorAcc <= errorAccaux) {
            y0++;
        }
        x0 += xDir;
        weighting = errorAcc >> 8;

        Uint32 clrBackGround = getPixel(x0, y0);
        Uint8 rb = color.getColorComponent(clrBackGround, 'r');
        Uint8 gb = color.getColorComponent(clrBackGround, 'g');
        Uint8 bb = color.getColorComponent(clrBackGround, 'b');
        double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

        Uint8 rr = (rb > rl ? ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (rb - rl) + rl)) : ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (rl - rb) + rb)));
        Uint8 gr = (gb > gl ? ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (gb - gl) + gl)) : ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (gl - gb) + gb)));
        Uint8 br = (bb > bl ? ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (bb - bl) + bl)) : ((Uint8)(((double)(grayl < grayb ? weighting : (weighting ^ 255))) / 255.0 * (bl - bb) + bb)));
        setPixel(x0, y0, color.RGB(rr, gr, br));

        clrBackGround = getPixel(x0, y0 + 1);
        rb = color.getColorComponent(clrBackGround, 'r');
        gb = color.getColorComponent(clrBackGround, 'g');
        bb = color.getColorComponent(clrBackGround, 'b');
        grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

        rr = (rb > rl ? ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (rb - rl) + rl)) : ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (rl - rb) + rb)));
        gr = (gb > gl ? ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (gb - gl) + gl)) : ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (gl - gb) + gb)));
        br = (bb > bl ? ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (bb - bl) + bl)) : ((Uint8)(((double)(grayl < grayb ? (weighting ^ 255) : weighting)) / 255.0 * (bl - bb) + bb)));
        setPixel(x0, y0 + 1, color.RGB(rr, gr, br));
    }

    setPixel(x1, y1, color);
}

void Line::bresenham(int x1, int y1, int x2, int y2, int r, int g, int b) {
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

    dx = x2 - x1;
    dy = y2 - y1;

    dx1 = std::abs(dx);
    dy1 = std::abs(dy);

    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;

    if (dy1 <= dx1) {
        if (dx >= 0) {
            x = x1;
            y = y1;
            xe = x2;
        }
        else {
            x = x2;
            y = y2;
            xe = x1;
        }
        setPixel(x, y, r, g, b);
        for (i = 0; x < xe; i++) {
            x = x + 1;
            if (px < 0) {
                px = px + 2 * dy1;
            }
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y = y + 1;
                }
                else {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            setPixel(x, y, r, g, b);
        }
    }
    else {
        if (dy >= 0) {
            x = x1;
            y = y1;
            ye = y2;
        }
        else {
            x = x2;
            y = y2;
            ye = y1;
        }
        setPixel(x, y, r, g, b);
        for (i = 0; y < ye; i++) {
            y = y + 1;
            if (py <= 0) {
                py = py + 2 * dx1;
            }
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    x = x + 1;
                }
                else {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            setPixel(x, y, r, g, b);
        }
    }
}