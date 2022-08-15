#pragma once
#include "GameObject.h"

class Tower : public GameObject
{
public:
    Tower(vec2 Position, const string& TowerName);
    static void DrawTowerOverlay(string TextureName, sf::RenderWindow& Renderer, bool IsBlocked);
    void Update(float DeltaTime);
    void Render(sf::RenderWindow& Renderer);
private:
    string m_Name;
    vec2i  m_TextureSize;
};

