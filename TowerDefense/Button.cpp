#include "Button.h"
#include "Engine.h"

Button::Button(string TextureName, vec2i Position, vec2i Size, function<void()> Function)
{
    m_TextureName = TextureName;
    m_Position = Position;
    m_Size = Size;
    m_Function = Function;
}

void Button::Render(sf::RenderWindow& Renderer)
{
    if (IsCursorOnButton())
    {
        Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TextureName).c_str(), vec2i(m_Position.x - 5, m_Position.y + 5), vec2i(m_Size.x, m_Size.y));
    }
    else Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TextureName).c_str(), vec2i(m_Position.x, m_Position.y), vec2i(m_Size.x, m_Size.y));
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
    vec2i MousePos = Engine::GetSingleton()->GetMousePos();

    vec2i TextureCenter = m_Position + m_Size / 2;
    vec2i TextureTopLeft = TextureCenter - m_Size / 2;
    vec2i TextureBottomLeft = TextureCenter + m_Size / 2;

    if (MousePos.x >= TextureTopLeft.x &&  MousePos.x <= TextureBottomLeft.x)
    {
        if (MousePos.y >= TextureTopLeft.y && MousePos.y <= TextureBottomLeft.y)
        {
            return true;
        }
    }

    return false;
}