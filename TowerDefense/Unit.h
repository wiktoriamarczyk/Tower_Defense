#pragma once
#include "GameObject.h"

class Unit : public GameObject
{
public:
    Unit(vec2i Position, const string& UnitName);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    void MoveTo(vec2 TargetPosition);
private:
    vec2 m_TargetPosition;
    float m_Speed = 60.f;
};