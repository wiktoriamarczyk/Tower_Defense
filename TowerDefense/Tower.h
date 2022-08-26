#pragma once
#include "GameObject.h"
#include "InGameState.h"
#include "Unit.h"

class Tower : public GameObject
{
public:
    Tower(InGameState& Game, vec2 Position, const string& TowerName, int Cost);
    static void DrawTowerOverlay(string TextureName, sf::RenderWindow& Renderer, bool IsBlocked);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    bool IsCursorOnButton()const;
    void Shoot(vec2 StartingPosition, shared_ptr<Unit> Target);

    bool IsPicked()const;
    int GetPrize()const;

private:
    InGameState&    m_Game;
    int             m_Cost = 0;
    vec2i           m_TextureSize;
    float           m_DetectionRadius = 170.f;
    sf::CircleShape m_DetectionArea;
    bool            m_Picked = false;

    float           m_ShootingTimer = 1.f;

};

