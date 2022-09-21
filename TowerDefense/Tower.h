#pragma once
#include "GameObject.h"
#include "InGameState.h"
#include "Unit.h"

class Tower : public GameObject
{
public:
    Tower(InGameState& Game, vec2 Position);
    static void DrawTowerOverlay(string TextureName, sf::RenderWindow& Renderer, bool IsBlocked);
    shared_ptr<Tower> GetSelf() {return static_pointer_cast<Tower>(GameObject::GetSelf());};
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    bool IsCursorOverObject()const override;
    bool FillToolTip(ToolTip& MyToolTip)const override;
    void Initialize(const Definition& Def);
    void Shoot(shared_ptr<Unit> Target);
    void LvlUp();

    bool IsPicked()const;
    float GetPrize()const;
    int GetLvl()const;
    int GetKills()const;
    float GetMoneyEarned()const;
    float GetPrizeForUpgrade();

    void SetKills(int Value);
    void SetMoneyEarned(float Value);
    void PickTower(bool Value);

private:
    InGameState&    m_Game;
    string          m_TextureName;
    float           m_Cost = 0;
    float           m_CostForUpgrade = 0;
    int             m_Lvl = 1;
    float           m_ShootInterval = 0;
    vec2i           m_TextureSize;
    float           m_DetectionRadius = 0;
    sf::CircleShape m_DetectionArea;
    bool            m_Picked = false;
    int             m_UnitsKilled = 0;
    int             m_MoneyEarned = 0;
    Damage          m_Damage;
    Damage          m_StartingDamageValues;

    float           m_ShootingTimer = 0;
};

