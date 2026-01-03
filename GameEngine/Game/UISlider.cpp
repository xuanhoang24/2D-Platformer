#include "../Game/UISlider.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/TTFont.h"
#include <sstream>

UISlider::UISlider()
{
    m_rect = { 0, 0, 0, 0 };
    m_value = 50; // Default 50%
    m_hovered = false;
    m_dragging = false;
    m_dragOffset = 0;
}

void UISlider::SetRect(int _x, int _y, int _width, int _height)
{
    m_rect.x = _x;
    m_rect.y = _y;
    m_rect.w = _width;
    m_rect.h = _height;
}

void UISlider::UpdateHover(int _mouseX, int _mouseY)
{
    // Check if mouse within slider bounds
    bool withinX = (_mouseX >= m_rect.x) && (_mouseX <= m_rect.x + m_rect.w);
    bool withinY = (_mouseY >= m_rect.y) && (_mouseY <= m_rect.y + m_rect.h);
    
    m_hovered = withinX && withinY;
}

bool UISlider::HandleInput(SDL_Event& _event, int _mouseX, int _mouseY)
{
    bool valueChanged = false;
    
    // Handle mouse button press
    if (_event.type == SDL_MOUSEBUTTONDOWN && _event.button.button == SDL_BUTTON_LEFT)
    {
        if (m_hovered)
        {
            m_dragging = true;
            
            // Calculate handle position and drag offset
            int handleWidth = 4;
            int handleX = m_rect.x + (m_value * (m_rect.w - handleWidth)) / 100 + (handleWidth / 2);
            m_dragOffset = _mouseX - handleX;
        }
    }
    // Handle mouse button release
    else if (_event.type == SDL_MOUSEBUTTONUP && _event.button.button == SDL_BUTTON_LEFT)
    {
        m_dragging = false;
        m_dragOffset = 0;
    }
    // Handle mouse movement while dragging
    else if (_event.type == SDL_MOUSEMOTION && m_dragging)
    {
        // Calculate new slider value based on mouse position
        int handleWidth = 4;
        int trackWidth = m_rect.w - handleWidth;
        int mouseRelativeX = _mouseX - m_dragOffset - m_rect.x - (handleWidth / 2);
        
        // Convert mouse position to percentage (0-100)
        int newValue = (mouseRelativeX * 100) / trackWidth;
        
        // Clamp value between 0 and 100
        int oldValue = m_value;
        m_value = max(0, min(100, newValue));
        
        // Check if value actually changed
        valueChanged = (oldValue != m_value);
    }
    
    return valueChanged;
}

void UISlider::Render(Renderer* _renderer, TTFont* _font, const char* _label)
{
    SDL_Renderer* renderer = _renderer->GetRenderer();
    SDL_Color white = { 255, 255, 255, 255 };
    
    // Label text
    int labelWidth, labelHeight;
    _font->GetTextSize(_label, &labelWidth, &labelHeight);
    SDL_Point labelPos = { 
        m_rect.x + (m_rect.w - labelWidth) / 2, 
        m_rect.y - 15 
    };
    _font->Write(renderer, _label, white, labelPos);
    
    // Slider track
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    int trackMargin = 2;
    SDL_Rect track = { 
        m_rect.x + trackMargin, 
        m_rect.y + m_rect.h / 2 - 1, 
        m_rect.w - (trackMargin * 2), 
        2 
    };
    SDL_RenderFillRect(renderer, &track);
    
    int handleWidth = 4;
    int handleHeight = (m_rect.h * 9) / 10;
    int handleX = m_rect.x + (m_value * (m_rect.w - handleWidth)) / 100;
    int handleY = m_rect.y + (m_rect.h - handleHeight) / 2;
    
    // Handle color
    SDL_Color handleColor;
    if (m_hovered || m_dragging)
    {
        handleColor = { 255, 220, 50, 255 }; // Yellow when active
    }
    else
    {
        handleColor = { 200, 200, 200, 255 }; // Gray when inactive
    }
    
    // Slider handle
    SDL_SetRenderDrawColor(renderer, handleColor.r, handleColor.g, handleColor.b, handleColor.a);
    SDL_Rect handle = { handleX, handleY, handleWidth, handleHeight };
    SDL_RenderFillRect(renderer, &handle);
    
    // Percentage text
    std::stringstream percentageText;
    percentageText << m_value << "%";
    
    int textWidth, textHeight;
    _font->GetTextSize(percentageText.str().c_str(), &textWidth, &textHeight);
    SDL_Point textPos = { 
        m_rect.x + (m_rect.w - textWidth) / 2, 
        m_rect.y + m_rect.h + 2 
    };
    _font->Write(renderer, percentageText.str().c_str(), white, textPos);
}

void UISlider::Reset()
{
    m_hovered = false;
    m_dragging = false;
    m_dragOffset = 0;
}