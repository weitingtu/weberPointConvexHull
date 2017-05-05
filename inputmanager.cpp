#include "inputmanager.h"
#include <stdlib.h>

static const int _default_width(600);
static const int _default_height(300);

InputManager::InputManager() :
    _width(_default_width),
    _height(_default_height),
    _inputs()
{

}

void InputManager::generate(int size)
{
    clear();
    for(int i = 0; i < size; ++i)
    {
        _inputs.push_back(QPointF(rand() % _width, rand() % _height));
    }
}

void InputManager::hexagonal()
{

}

void InputManager::clear()
{
    _inputs.clear();
}
