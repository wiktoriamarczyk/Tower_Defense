#include "Tower.h"
#include "Engine.h"

Tower::Tower(vec2 Position, const string& TowerName)
{
    m_TowerName = TowerName;
    m_ObjectSize = vec2(64,64);
    m_StartingPointPosition.x = int(Position.x)/64*64;
    m_StartingPointPosition.y = int(Position.y)/64*64;

    m_TextureSize = Engine::GetSingleton()->GetTextureSize(("../Data/" + TowerName + ".png").c_str());
}

void Tower::Update(float DeltaTime)
{
}

void Tower::Render(SDL_Renderer* pRenderer)
{
    vec2 TextureCenter = m_StartingPointPosition + m_ObjectSize / 2;
    vec2 TextureTopLeft = TextureCenter - (vec2)m_TextureSize / 2;

    Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TowerName + ".png").c_str(), TextureTopLeft.x, TextureTopLeft.y, 0, 0);
}