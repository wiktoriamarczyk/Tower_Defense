#include "Button.h"
#include "Engine.h"
#include "ToolTip.h"

Button::Button(string TextureName, vec2 Position, vec2 Pivot, function<void()> Function, bool Move)
{
    m_TextureName = TextureName;
    m_ID = Engine::GetSingleton()->GenerateTextureID(m_TextureName);
    SetSize(Engine::GetSingleton()->GetTextureSize(m_ID));
    m_Layer = eGraphicLayer::UI;
    SetPosition(Position);
    m_Pivot = Pivot;
    m_Function = Function;
    m_Move = Move;
}

void Button::Render(sf::RenderWindow& Renderer)
{
    if (IsCursorOverObject() && m_Move)
    {
        Engine::GetSingleton()->DisplayTexture(m_ID, vec2i(GetPosition().x - 5, GetPosition().y + 5), DisplayParameters{.Pivot = m_Pivot});
    }
    else 
        Engine::GetSingleton()->DisplayTexture(m_ID, vec2i(GetPosition().x, GetPosition().y), DisplayParameters{.Pivot = m_Pivot});
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

    vec2 textureTopLeft = GetPosition() - m_Pivot * GetSize();
    vec2 textureBottomRight = textureTopLeft + GetSize();

    if (mousePos.x >= textureTopLeft.x &&  mousePos.x <= textureBottomRight.x)
    {
        if (mousePos.y >= textureTopLeft.y && mousePos.y <= textureBottomRight.y)
        {
            return true;
        }
    }

    return false;
}

bool Button::FillToolTip(ToolTip& MyToolTip)const
{
    if (m_ToolTip.empty())
        return false;

    for (size_t i = 0; i < m_ToolTip.size(); ++i)
    {
        MyToolTip.AddToolTipLine(m_ToolTip[i].Text, m_ToolTip[i].Size, m_ToolTip[i].Parameters);
    }

    return true;
}

void Button::SetToolTipText(vector<TextLineData> ToolTip)
{
    m_ToolTip = ToolTip;
}

void Button::ClearToolTip()
{
    if (!m_ToolTip.empty())
        m_ToolTip.clear();
}

bool TowerButton::FillToolTip(ToolTip& MyToolTip)const
{
    if (!m_pDef)
        return false;

    MyToolTip.AddToolTipLine(m_pDef->GetStringValue("Name") + " ");
    MyToolTip.AddToolTipLine("🏰", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color::Yellow, .ContinueLastLine = true});

    MyToolTip.AddToolTipLine("Cost: " + ToString(m_pDef->GetIntValue("Cost")) + " ");
    MyToolTip.AddToolTipLine("💰", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color::Yellow, .ContinueLastLine = true});

    MyToolTip.AddToolTipLine("Fire Dmg: " + ToString(m_pDef->GetFloatValue("FireDamage")) + " ");
    MyToolTip.AddToolTipLine("🔥", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color(255, 95, 31), .ContinueLastLine = true});

    MyToolTip.AddToolTipLine("Lightning Dmg: " + ToString(m_pDef->GetFloatValue("LightningDamage")) + " ");
    MyToolTip.AddToolTipLine("⚡", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color::Yellow, .ContinueLastLine = true});

    MyToolTip.AddToolTipLine("Ice Dmg: " + ToString(m_pDef->GetFloatValue("IceDamage")) + " ");
    MyToolTip.AddToolTipLine("❄", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color(0,191,255), .ContinueLastLine = true});
    
    MyToolTip.AddToolTipLine("Detection Radius: " + ToString(m_pDef->GetIntValue("DetectionRadius")) + " ");
    MyToolTip.AddToolTipLine("📡", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color(169, 169, 169), .ContinueLastLine = true});

    MyToolTip.AddToolTipLine("Shoot Interval:  " + ToString(m_pDef->GetFloatValue("ShootInterval")) + " ");
    MyToolTip.AddToolTipLine("💨", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .ContinueLastLine = true});


    return true;
}

void TowerButton::SetDefinition(const Definition* pDef)
{
    m_pDef = pDef;
}
