#pragma once
#include "GameObject.h"
#include "Unit.h"
#include "Tower.h"

class Shot : public GameObject
{
public:
    Shot(shared_ptr<Tower> Source, shared_ptr<Unit> Target, Damage DamageValue);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;

private:
    shared_ptr<Unit>  m_Target;
    shared_ptr<Tower> m_Source;
    vec2              m_NormalizedDir;
    float             m_Speed = 120.f;
    Damage            m_Damage;
};