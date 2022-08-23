#pragma once
#include "GameObject.h"

class Shot : public GameObject
{
public:
    Shot(vec2 Position, vec2 TargetPosition);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;

private:
    vec2  m_DirectionVec;
    float m_Speed = 1200.f;
};