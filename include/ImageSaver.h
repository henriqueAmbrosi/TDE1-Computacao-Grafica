#ifndef IMAGESAVER_H
#define IMAGESAVER_H

#include <SDL2/SDL.h>
#include <string>

class ImageSaver
{
public:
    static bool saveCanvas(SDL_Surface* surface);

    static bool saveCanvasToFile(SDL_Surface* surface, const std::string& filePath);

private:
    static std::string getPicturesDirectory();
    static long long generateSaveId();
};

#endif // IMAGESAVER_H