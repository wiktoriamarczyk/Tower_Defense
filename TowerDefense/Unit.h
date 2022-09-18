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
    bool IsCursorOverObject()const override;
    bool FillToolTip(ToolTip& MyToolTip)const override;
    void MoveTo(vector<vec2> TargetPosition);
    void Initialize(const Definition& Def);
    void OnHit(Damage DamageValue);

    bool GetDamageStatus()const;
    int GetHP()const;
    int GetMaxHP()const;
    float GetMoneyForKill()const;

    void SetDamageStatus(bool Status);
    void SetHP(int Value);

private:
    string       m_TextureName;
    vector<vec2> m_TargetPositions;
    float        m_Speed = 10.f;
    float        m_HP = 100.f;
    float        m_MaxHP = 100.f;
    float        m_MoneyForKill = 0;
    bool         m_IsHurt = false;
    Resistances  m_Resistances;

    float        m_HurtTimer = 0;
    float        m_DyingTimer = 0.5f;
};