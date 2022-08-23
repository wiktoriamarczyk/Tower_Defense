#pragma once
#include "GameObject.h"
#include "InGameState.h"

class Tower : public GameObject
{
public:
    Tower(InGameState& Game, vec2 Position, const string& TowerName);
    static void DrawTowerOverlay(string TextureName, sf::RenderWindow& Renderer, bool IsBlocked);
    void Update(float DeltaTime);
    void Render(sf::RenderWindow& Renderer);
    void Shoot(vec2 TargetPosition);
private:
    InGameState&    m_Game;
    vec2i           m_TextureSize;
    float           m_DetectionRadius = 170.f;
    sf::CircleShape m_DetectionArea;
    float           m_ShootingTimer = 1.f;

};

