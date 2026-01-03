#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include "../Core/StandardIncludes.h"

class Renderer;
class TTFont;

class UISlider
{
public:
    UISlider();
    
    void SetRect(int _x, int _y, int _width, int _height);
    void SetRect(SDL_Rect _rect) { m_rect = _rect; }
    SDL_Rect& GetRect() { return m_rect; }
    
    void SetValue(int _value) { m_value = max(0, min(100, _value)); }
    int GetValue() const { return m_value; }
    
    bool IsHovered() const { return m_hovered; }
    bool IsDragging() const { return m_dragging; }
    
    // Update hover and drag state based on mouse position
    void UpdateHover(int _mouseX, int _mouseY);
    
    // Handle mouse events for dragging
    bool HandleInput(SDL_Event& _event, int _mouseX, int _mouseY);
    
    // Render the slider
    void Render(Renderer* _renderer, TTFont* _font, const char* _label);
    
    // Reset state
    void Reset();

private:
    SDL_Rect m_rect;
    int m_value; // 0-100
    bool m_hovered;
    bool m_dragging;
    int m_dragOffset;
};

#endif // UI_SLIDER_H