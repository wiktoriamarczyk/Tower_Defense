#include "Button.h"
#include "Engine.h"

Button::Button(string TextureName, vec2 Position, vec2i Size, function<void()> Function, bool Move)
{
    m_TextureName = TextureName;
    SetPosition(Position);
    SetSize(Size);
    m_Function = Function;
    m_Layer = eGraphicLayer::UI;
    m_Move = Move;
}

void Button::Render(sf::RenderWindow& Renderer)
{
    if (IsCursorOverObject() && m_Move) 
    {
        Engine::GetSingleton()->DisplayTexture(m_TextureName, vec2i(GetPosition().x - 5, GetPosition().y + 5));
    }
    else Engine::GetSingleton()->DisplayTexture(m_TextureName, vec2i(GetPosition().x, GetPosition().y));
}

bool Button::OnMouseButtonDown(int Button)
{
    if (IsCursorOverObject())
    {
        m_Function();
        return true;
    }
    return false;
}

bool Button::IsCursorOverObject()const
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

vector<string> Button::GetToolTip()const
{
    return m_ToolTip;
}

void Button::SetToolTipText(vector<string> ToolTip)
{
    m_ToolTip = ToolTip;
}

vector<string> TowerButton::GetToolTip()const
{
    if (!IsCursorOverObject())
        return {};

    vector<string> tmp;

    tmp.push_back(m_pDef->GetStringValue("Name"));
    tmp.push_back(ToString(m_pDef->GetIntValue("Cost")));
    tmp.push_back(ToString(m_pDef->GetFloatValue("FireDamage")));
    tmp.push_back(ToString(m_pDef->GetFloatValue("LightningDamae")));
    tmp.push_back(ToString(m_pDef->GetFloatValue("IceDamage")));
    tmp.push_back(ToString(m_pDef->GetIntValue("DetectionRadius")));
    tmp.push_back(ToString(m_pDef->GetFloatValue("ShootInterval")));

    return tmp;
}

void TowerButton::SetDefinition(const Definition* pDef)
{
    m_pDef = pDef;
}
