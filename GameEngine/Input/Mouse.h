#ifndef MOUSE_H
#define MOUSE_H

#include "../Core/StandardIncludes.h"

class Mouse
{
public:
    // Constructors/ Destructors
    Mouse();
    virtual ~Mouse();

    /// Accessors
    Point* GetPos() { return &m_pos; }
    Point* GetPosPrev() { return &m_posPrev; }
    bool GetButLDown() { return m_butLDown; }
    bool GetButRDown() { return m_butRDown; }
    bool GetButMDown() { return m_butMDown; }

    // Methods
    bool Moved(SDL_Event _event, Point& _pos);
    void ProcessButtons(SDL_Event _event);

private:
    // Members
    Point m_pos;
    Point m_posPrev;
    bool m_butLDown;
    bool m_butMDown;
    bool m_butRDown;

};

#endif // MOUSE_H