#include "Tower.h"

Tower::Tower(vec2 Position, SDL_Texture* TowerTexture)
{
    m_ObjectSize = vec2(64,64);
    m_pTowerTexture = TowerTexture;
    m_StartingPointPosition.x = int(Position.x)/64*64;
    m_StartingPointPosition.y = int(Position.y)/64*64;
}

void Tower::Update(float DeltaTime)
{
}

void Tower::Render(SDL_Renderer* pRenderer)
{
    vec2 TextureSize(128,128);
    vec2 TextureCenter = m_StartingPointPosition + m_ObjectSize / 2;
    vec2 TextureTopLeft = TextureCenter - TextureSize / 2;

    SDL_Rect TowerRect = { TextureTopLeft.x, TextureTopLeft.y, TextureSize.x, TextureSize.y };
    SDL_RenderCopy(pRenderer, m_pTowerTexture, NULL, &TowerRect);
}