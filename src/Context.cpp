#include "Context.h"

Context* Context::instance = nullptr;

Context::Context() {
    this->window_surface = nullptr;
    this->pRenderer = nullptr;
    this->selectedColor = Color(0, 0, 0);
    this->selectedTool = Tool::RECTANGLE;
    this->selectedFigure = nullptr;
    this->menuVisible = true;
    this->resizing = false;
    this->resizeAnchor = Anchor::NONE;
    this->resizeStartScaleX = 1.0f;
    this->resizeStartScaleY = 1.0f;
    this->resizeRefWidth = 1.0f;
    this->resizeRefHeight = 1.0f;
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

bool Context::isResizing() {
    return this->resizing;
}

void Context::setIsResizing(bool isResizing) {
    this->resizing = isResizing;
}

Point Context::getInitalResizePoint() {
    return this->initialResizePoint;
}

void Context::setInitialResizePoint(Point initalResizePoint) {
    this->initialResizePoint = initalResizePoint;
}

void Context::setResizeAnchor(Anchor anchor) {
    this->resizeAnchor = anchor;
}

Anchor Context::getResizeAnchor() {
    return this->resizeAnchor;
}

void Context::setResizeStartScale(float x, float y) {
    this->resizeStartScaleX = x;
    this->resizeStartScaleY = y;
}

float Context::getResizeStartScaleX() {
    return this->resizeStartScaleX;
}

float Context::getResizeStartScaleY() {
    return this->resizeStartScaleY;
}

void Context::setResizeRefSize(float width, float height) {
    this->resizeRefWidth = width;
    this->resizeRefHeight = height;
}

float Context::getResizeRefWidth() {
    return this->resizeRefWidth;
}

float Context::getResizeRefHeight() {
    return this->resizeRefHeight;
}