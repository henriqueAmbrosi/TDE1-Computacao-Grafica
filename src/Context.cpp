#include "Context.h"

Context* Context::_instance = nullptr;

Context::Context() {
    this->_window_surface = nullptr;
    this->_pRenderer = nullptr;
    this->selectedColor = Color(255, 0, 0);
    this->selectedTool = Tool::RECTANGLE;
    this->selectedFigure = nullptr;
    this->menuVisible = true;
}

Context::~Context()
{
}

Context* Context::getInstance()
{
    if (_instance == nullptr) {
        _instance = new Context();
    }
    return _instance;
}

void Context::setWindowSurface(SDL_Surface* window_surface)
{
    _window_surface = window_surface;
}

void Context::setRenderer(SDL_Renderer* pRenderer)
{
    _pRenderer = pRenderer;
}

SDL_Surface* Context::getWindowSurface()
{
    return _window_surface;
}

SDL_Renderer* Context::getRenderer()
{
    return _pRenderer;
}

Color Context::getSelectedColor()
{
    return selectedColor;
}

void Context::setSelectedColor(Color& color)
{
    selectedColor = color;
}

Tool Context::getSelectedTool()
{
    return selectedTool;
}

void Context::setSelectedTool(Tool tool)
{
    selectedTool = tool;
}

std::shared_ptr<Shape> Context::getSelectedFigure()
{
    return selectedFigure;
}

void Context::setSelectedFigure(std::shared_ptr<Shape> figure)
{
    selectedFigure = figure;
}

std::list<std::shared_ptr<Shape>>& Context::getFigures()
{
    return figures;
}

void Context::addFigure(std::shared_ptr<Shape> figure)
{
    figures.push_back(figure);
}

void Context::removeFigure(std::shared_ptr<Shape> figure)
{
    if (figure) {
        figures.remove(figure);
    }
}

bool Context::isMenuVisible() {
    return this->menuVisible;
}

void Context::setMenuVisible(bool menuVisible) {
    this->menuVisible = menuVisible;
}