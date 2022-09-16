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
    vector<string> GetToolTip()const override;
    void Initialize(const Definition& Def);
    void Shoot(shared_ptr<Unit> Target);

    bool IsPicked()const;
    int GetPrize()const;
    int GetLvl()const;
    int GetKills()const;

    void SetLvl(int Lvl);
    void SetKills(int Value);

private:
    InGameState&    m_Game;
    string          m_TextureName;
    int             m_Cost = 0;
    int             m_Lvl = 1;
    float           m_ShootInterval = 0;
    vec2i           m_TextureSize;
    float           m_DetectionRadius = 0;
    sf::CircleShape m_DetectionArea;
    bool            m_Picked = false;
    int             m_UnitsKilled = 0;
    Damage          m_Damage;

    float           m_ShootingTimer = 0;

};

