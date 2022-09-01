#include "Button.h"
#include "Engine.h"

Button::Button(string TextureName, vec2i Position, vec2i Size, function<void()> Function)
{
    m_TextureName = TextureName;
    SetPosition(Position);
    SetSize(Size);
    m_Function = Function;
    m_Layer = eGraphicLayer::UI;
}

void Button::Render(sf::RenderWindow& Renderer)
{
    if (IsCursorOnButton())
    {
        Engine::GetSingleton()->DisplayTexture(m_TextureName, vec2i(GetPosition().x - 5, GetPosition().y + 5));
    }
    else Engine::GetSingleton()->DisplayTexture(m_TextureName, vec2i(GetPosition().x, GetPosition().y));
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

    vec2 textureCenter = GetPosition() + GetSize() / 2;
    vec2 textureTopLeft = textureCenter - GetSize() / 2;
    vec2 textureBottomRight = textureCenter + GetSize() / 2;

    if (mousePos.x >= textureTopLeft.x &&  mousePos.x <= textureBottomRight.x)
    {
        if (mousePos.y >= textureTopLeft.y && mousePos.y <= textureBottomRight.y)
        {
            return true;
        }
    }

    return false;
}