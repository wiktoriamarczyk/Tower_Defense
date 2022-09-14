#pragma once
#include "GameObject.h"
#include "Unit.h"

class Shot : public GameObject
{
public:
    Shot(vec2 StartingPosition, shared_ptr<Unit> Target, Damage DamageValue);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;

private:
    shared_ptr<Unit> m_Target;
    vec2             m_NormalizedDir;
    float            m_Speed = 1200.f;
    Damage           m_Damage;
};