#pragma once
#include "GameObject.h"

class Tower : public GameObject
{
public:
    Tower(vec2 Position, SDL_Texture* TowerTexture);
    void Update(float DeltaTime);
    void Render(SDL_Renderer* pRenderer);
private:
    SDL_Texture* m_pTowerTexture = nullptr;
    vec2i        m_TextureSize;
};

