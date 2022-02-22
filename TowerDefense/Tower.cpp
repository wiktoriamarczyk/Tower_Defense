#include "Tower.h"
#include "Engine.h"

Tower::Tower(vec2 Position, const string& TowerName)
{
    m_TowerName = TowerName;
    m_ObjectSize = vec2i(64,64);
    m_Position.x = int(Position.x)/64*64;
    m_Position.y = int(Position.y)/64*64;

    m_TextureSize = Engine::GetSingleton()->GetTextureSize(("../Data/" + TowerName + ".png").c_str());
}

void Tower::Update(float DeltaTime)
{
}

void Tower::Render(SDL_Renderer* pRenderer)
{
    vec2i TextureCenter = m_Position + m_ObjectSize / 2;
    vec2i TextureTopLeft = TextureCenter - m_TextureSize / 2;

    Engine::GetSingleton()->DisplayTexture(("../Data/" + m_TowerName + ".png").c_str(), TextureTopLeft.x, TextureTopLeft.y);
}