#include "Tower.h"

Tower::Tower(vec2 Position, SDL_Texture* TowerTexture)
{
    m_ObjectSize = vec2(64,64);
    m_pTowerTexture = TowerTexture;
    m_StartingPointPosition.x = int(Position.x)/64*64;
    m_StartingPointPosition.y = int(Position.y)/64*64;

    if(SDL_QueryTexture(m_pTowerTexture, nullptr, nullptr, &m_TextureSize.x, &m_TextureSize.y) != 0)
    {
        cout << SDL_GetError();
        throw 1;
    }
}

void Tower::Update(float DeltaTime)
{
}

void Tower::Render(SDL_Renderer* pRenderer)
{
    vec2 TextureCenter = m_StartingPointPosition + m_ObjectSize / 2;
    vec2 TextureTopLeft = TextureCenter - (vec2)m_TextureSize / 2;

    SDL_Rect TowerRect = { TextureTopLeft.x, TextureTopLeft.y, m_TextureSize.x, m_TextureSize.y };
    SDL_RenderCopy(pRenderer, m_pTowerTexture, NULL, &TowerRect);
}