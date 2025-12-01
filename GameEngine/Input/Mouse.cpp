#include "../Input/Mouse.h"

Mouse::Mouse()
{
    m_pos = Point();
    m_posPrev = Point();
    m_butLDown = false;
    m_butMDown = false;
    m_butRDown = false;
}

Mouse::~Mouse()
{
}

bool Mouse::Moved(SDL_Event _event, Point& _pos)
{
    if (_event.type != SDL_MOUSEMOTION) return false;

    m_pos.X = _event.motion.x;
    m_pos.Y = _event.motion.y;
    _pos = m_pos;
    m_posPrev = m_pos;

    return true;
}

void Mouse::ProcessButtons(SDL_Event _event)
{
    if (_event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (_event.button.button == SDL_BUTTON_LEFT) m_butLDown = true;
        if (_event.button.button == SDL_BUTTON_MIDDLE) m_butMDown = true;
        if (_event.button.button == SDL_BUTTON_RIGHT) m_butRDown = true;
    }
    else if (_event.type == SDL_MOUSEBUTTONUP)
    {
        if (_event.button.button == SDL_BUTTON_LEFT) m_butLDown = false;
        if (_event.button.button == SDL_BUTTON_MIDDLE) m_butMDown = false;
        if (_event.button.button == SDL_BUTTON_RIGHT) m_butRDown = false;
    }
}