#include "Tower.h"
#include "Engine.h"

Tower::Tower(vec2 Position, const string& TowerName)
{
    m_Name = TowerName;
    m_Size = vec2i(CELL_SIZE,CELL_SIZE);
    m_Position.x = int(Position.x)/CELL_SIZE * CELL_SIZE;
    m_Position.y = int(Position.y)/CELL_SIZE * CELL_SIZE;

    m_TextureSize = Engine::GetSingleton()->GetTextureSize(("../Data/" + TowerName + ".png").c_str());
}

void Tower::DrawTowerOverlay(string TextureName, sf::RenderWindow& Renderer, bool IsBlocked)
{
    auto MousePos = (Engine::GetSingleton()->GetMousePos() / CELL_SIZE) * CELL_SIZE;
    
    if (IsBlocked)
        Engine::GetSingleton()->DisplayTexture(("../Data/" + TextureName + ".png").c_str(), MousePos + vec2i(CELL_SIZE/2, CELL_SIZE/2), DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot = vec2(0.5,0.5), .DrawColor = sf::Color::Red});
    else
        Engine::GetSingleton()->DisplayTexture(("../Data/" + TextureName + ".png").c_str(), MousePos + vec2i(CELL_SIZE/2, CELL_SIZE/2), DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot = vec2(0.5,0.5)});
}

void Tower::Update(float DeltaTime)
{
}

void Tower::Render(sf::RenderWindow& Renderer)
{
    vec2 TextureCenter = m_Position + m_Size / 2;
    vec2 TextureTopLeft = TextureCenter - m_TextureSize / 2;

    Engine::GetSingleton()->DisplayTexture(("../Data/" + m_Name + ".png").c_str(), TextureTopLeft);
}