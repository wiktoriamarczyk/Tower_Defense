#pragma once
#include "GameObject.h"

class Unit : public GameObject
{
public:
    Unit(vec2 Position, const string& UnitName);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    void MoveTo(vector<vec2> TargetPosition);

    bool GetDamageStatus()const;
    int GetLifeCount()const;
    
    void SetDamageStatus(bool Status);
    void SetLifeCount(int Value);

private:
    vector<vec2> m_TargetPositions;
    float        m_Speed = 200.f;
    int          m_LifeCount = 2;
    bool         m_IsHurt = false;
    float        m_HurtTimer = 0;
};