#include "Context.h"

Context* Context::instance = nullptr;

Context::Context() {
    this->window_surface = nullptr;
    this->pRenderer = nullptr;
    this->selectedColor = Color(0, 0, 0);
    this->selectedTool = Tool::RECTANGLE;
    this->selectedFigure = nullptr;
    this->menuVisible = true;
}

Context::~Context() {
}

Context* Context::getInstance() {
    if (instance == nullptr) {
        instance = new Context();
    }
    return instance;
}

void Context::setWindowSurface(SDL_Surface* window_surface) {
    this->window_surface = window_surface;
}

void Context::setRenderer(SDL_Renderer* pRenderer) {
    this->pRenderer = pRenderer;
}

SDL_Surface* Context::getWindowSurface()  {
    return this->window_surface;
}

SDL_Renderer* Context::getRenderer()  {
    return this->pRenderer;
}

ToolMenu Context::getToolMenu()  {
    return this->toolMenu;
}

Color Context::getSelectedColor()  {
    return this->selectedColor;
}

void Context::setSelectedColor( Color& color) {
    this->selectedColor = color;
}

Tool Context::getSelectedTool()  {
    return this->selectedTool;
}

void Context::setSelectedTool(Tool tool) {
    this->selectedTool = tool;
}

std::shared_ptr<Shape> Context::getSelectedFigure()  {
    return this->selectedFigure;
}

void Context::setSelectedFigure(std::shared_ptr<Shape> figure) {
    this->selectedFigure = figure;
}

std::list<std::shared_ptr<Drawable>>& Context::getDrawables() {
    return this->drawables;
}

void Context::addDrawable(std::shared_ptr<Drawable> drawable) {
    this->drawables.push_back(drawable);
}

void Context::removeDrawable(std::shared_ptr<Drawable> drawable) {
    if (drawable) {
        this->drawables.remove(drawable);
    }
}

bool Context::isMenuVisible() {
    return this->menuVisible;
}

void Context::setMenuVisible(bool menuVisible) {
    this->menuVisible = menuVisible;
}