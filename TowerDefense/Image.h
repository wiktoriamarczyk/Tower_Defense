#pragma once
#include "GameObject.h"

class Image : public GameObject
{
public:
    Image(const string& Name, vec2 Position, vec2 Pivot);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    vector<string> GetToolTip()const override;
    bool IsCursorOverObject()const override;
    void Initialize(const string& Name);

private:
    vec2   m_Pivot;
    string m_ReferenceUnitName;
};