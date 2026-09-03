#include "ImageSaver.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

std::string ImageSaver::getPicturesDirectory()
{
    const char* home = nullptr;

#ifdef _WIN32
    home = getenv("USERPROFILE");
#else
    home = getenv("HOME");
#endif

    if (home == nullptr) {
        printf("Warning: Could not find HOME directory. Saving file on current folder.\n");
        return ".";
    }

    std::string homePath = std::string(home);

    std::string imagensPath = homePath + PATH_SEPARATOR + "Imagens";
    if (std::filesystem::exists(imagensPath)) {
        return imagensPath;
    }

    std::string picturesPath = homePath + PATH_SEPARATOR + "Pictures";
    if (std::filesystem::exists(picturesPath)) {
        return picturesPath;
    }

    return homePath;
}

long long ImageSaver::generateSaveId()
{
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime = *std::localtime(&currentTime);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d%H%M%S");

    return std::stoll(oss.str());
}

bool ImageSaver::saveCanvas(SDL_Surface* surface)
{
    if (surface == nullptr) {
        printf("Error: Surface is null, cannot save image.\n");
        return false;
    }

    std::string picturesDir = getPicturesDirectory();
    std::string fullPath = picturesDir + PATH_SEPARATOR + "imagem-" + std::to_string(generateSaveId()) + ".bmp";

    return saveCanvasToFile(surface, fullPath);
}

bool ImageSaver::saveCanvasToFile(SDL_Surface* surface, const std::string& filePath)
{
    if (surface == nullptr) {
        printf("Error: Surface is null.\n");
        return false;
    }

    if (SDL_SaveBMP(surface, filePath.c_str()) == 0) {
        printf("File saved successfully: %s\n", filePath.c_str());
        return true;
    } else {
        printf("Error saving file: %s\n", SDL_GetError());
        return false;
    }
}