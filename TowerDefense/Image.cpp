#include "Image.h"
#include "Engine.h"
#include "ToolTip.h"

Image::Image(const string& Name, vec2 Position, vec2 Pivot)
{
    m_Name = Name;
    SetPosition(Position);
    m_Pivot = Pivot;
}

void Image::Update(float DeltaTime)
{

}

void Image::Render(sf::RenderWindow& Renderer)
{
    if (m_Name.empty())
        return;

    Engine::GetSingleton()->DisplayTexture((m_Name).c_str(), GetPosition(), DisplayParameters{.Pivot = m_Pivot});
}


 bool Image::FillToolTip(ToolTip& MyToolTip)const
{
    const Definition* pDef = Engine::GetSingleton()->FindDefinition(m_ReferenceUnitName);

    if (!pDef)
        return false;

    MyToolTip.AddToolTipLine(pDef->GetStringValue("Name"));
    MyToolTip.AddToolTipLine(pDef->GetStringValue("Ability"));

    return true;
}

bool Image::IsCursorOverObject()const
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

void Image::Initialize(const string& Name)
{
    m_ReferenceUnitName = string("/Definitions/" + Name + ".xml");
    m_Name = "/Images/" + Name + "Icon.png";
    m_Size = Engine::GetSingleton()->GetTextureSize(m_Name);
}