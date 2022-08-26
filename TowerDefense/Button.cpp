#include "Button.h"
#include "Engine.h"

Button::Button(string TextureName, vec2i Position, vec2i Size, function<void()> Function)
{
    m_TextureName = TextureName;
    m_Position = Position;
    m_Size = Size;
    m_Function = Function;
    m_Layer = eGraphicLayer::UI;
}

void Button::Render(sf::RenderWindow& Renderer)
{
    if (IsCursorOnButton())
    {
        Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TextureName).c_str(), vec2i(m_Position.x - 5, m_Position.y + 5));
    }
    else Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TextureName).c_str(), vec2i(m_Position.x, m_Position.y));
}

bool Button::OnMouseButtonDown(int Button)
{
    if (IsCursorOnButton())
    {
        m_Function();
        return true;
    }
    return false;
}

bool Button::IsCursorOnButton()const
{
    vec2i mousePos = Engine::GetSingleton()->GetMousePos();

    vec2 textureCenter = m_Position + m_Size / 2;
    vec2 textureTopLeft = textureCenter - m_Size / 2;
    vec2 textureBottomRight = textureCenter + m_Size / 2;

    if (mousePos.x >= textureTopLeft.x &&  mousePos.x <= textureBottomRight.x)
    {
        if (mousePos.y >= textureTopLeft.y && mousePos.y <= textureBottomRight.y)
        {
            return true;
        }
    }

    return false;
}