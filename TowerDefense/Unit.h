#pragma once
#include "GameObject.h"

class Unit : public GameObject
{
public:
    Unit(vec2i Position, const string& UnitName);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    void MoveTo(vector<vec2> TargetPosition);
private:
    vector<vec2> m_TargetPositions;
    float        m_Speed = 400.f;
};