#pragma once
#include "GameObject.h"
#include "Definition.h"

class Unit : public GameObject
{
public:
    Unit(vec2 Position);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    void MoveTo(vector<vec2> TargetPosition);
    void Initialize(const Definition& Def);

    bool GetDamageStatus()const;
    int GetHP()const;
    int GetMaxHP()const;
    
    void SetDamageStatus(bool Status);
    void SetHP(int Value);

private:
    string       m_TextureName;
    vector<vec2> m_TargetPositions;
    float        m_Speed = 10.f;
    float        m_HP = 100.f;
    float        m_MaxHP = 100.f;
    bool         m_IsHurt = false;
    float        m_HurtTimer = 0;
};