#include "Button.h"
#include "Engine.h"

Button::Button(string TextureName, vec2i Position, vec2i Size, function<void()> Function)
{
    m_TextureName = TextureName;
    m_Position = Position;
    m_ObjectSize = Size;
    m_Function = Function;
}

void Button::Render(SDL_Renderer* pRenderer)
{
    if (IsCursorOnButton())
    {
        Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TextureName).c_str(), m_Position.x - 5, m_Position.y + 5, m_ObjectSize.x, m_ObjectSize.y);
    }
    else Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TextureName).c_str(), m_Position.x, m_Position.y, m_ObjectSize.x, m_ObjectSize.y);
}

void Button::OnMouseButtonDown(int Button)
{
    if (IsCursorOnButton())
    {
        m_Function();
    }
}

bool Button::IsCursorOnButton()const
{
    vec2i MousePos = Engine::GetSingleton()->GetMousePos();

    vec2i TextureCenter = m_Position + m_ObjectSize / 2;
    vec2i TextureTopLeft = TextureCenter - m_ObjectSize / 2;
    vec2i TextureBottomLeft = TextureCenter + m_ObjectSize / 2;

    if (MousePos.x >= TextureTopLeft.x &&  MousePos.x <= TextureBottomLeft.x)
    {
        if (MousePos.y >= TextureTopLeft.y && MousePos.y <= TextureBottomLeft.y)
        {
            return true;
        }
    }

    return false;
}