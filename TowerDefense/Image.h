#pragma once
#include "GameObject.h"

class Image : public GameObject
{
public:
    Image(const string& Name, vec2 Position, vec2 Pivot);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool FillToolTip(ToolTip& MyToolTip)const override;
    bool IsCursorOverObject()const override;
    void InitializeUnitIcon(const string& UnitName);

private:
    vec2      m_Pivot;
    string    m_ReferenceUnitName;
};