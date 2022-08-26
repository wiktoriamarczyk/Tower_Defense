#include "Tower.h"
#include "Engine.h"
#include "Unit.h"

Tower::Tower(InGameState& Game, vec2 Position, const string& TowerName, int Cost) : m_Game(Game)
{
    m_DetectionArea.setRadius(m_DetectionRadius);

    m_Name = TowerName;
    m_Cost = Cost;
    m_Size = vec2i(CELL_SIZE,CELL_SIZE);
    m_Position.x = (int(Position.x)/CELL_SIZE * CELL_SIZE) + CELL_SIZE / 2;
    m_Position.y = (int(Position.y)/CELL_SIZE * CELL_SIZE) + CELL_SIZE / 2;

    m_TextureSize = Engine::GetSingleton()->GetTextureSize(("../Data/" + TowerName + ".png").c_str());
}

void Tower::DrawTowerOverlay(string TextureName, sf::RenderWindow& Renderer, bool IsBlocked)
{
    auto mousePos = (Engine::GetSingleton()->GetMousePos() / CELL_SIZE) * CELL_SIZE;
    
    if (IsBlocked)
        Engine::GetSingleton()->DisplayTexture(("../Data/" + TextureName + ".png").c_str(), mousePos + vec2i(CELL_SIZE/2, CELL_SIZE/2), DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot = vec2(0.5,0.5), .DrawColor = sf::Color::Red});
    else
        Engine::GetSingleton()->DisplayTexture(("../Data/" + TextureName + ".png").c_str(), mousePos + vec2i(CELL_SIZE/2, CELL_SIZE/2), DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot = vec2(0.5,0.5)});
}

void Tower::Update(float DeltaTime)
{
    vec2 objectTopLeft = m_Position - m_Size / 2;
    vec2 objectBottomRight = m_Position + m_Size / 2;

    m_ShootingTimer -= DeltaTime;

    // sprawdz, czy wroga jednostka znajduje sie w polu widzenia
    // wykorzystujac rownanie okregu o srodku w punkcie (a, b): (x-a)^2 + (y-b)^2 = r^2
    // srodek pola widzenia znajduje sie w centrum tekstury: m_Position

    vector<shared_ptr<Unit>> units = m_Game.GetObjects<Unit>();

    for (size_t i = 0; i < units.size(); ++i)
    {
        float checkIfInCircle = pow(units[i]->GetPosition().x - GetPosition().x, 2) + pow(units[i]->GetPosition().y - GetPosition().y, 2);

        if (checkIfInCircle <= pow(m_DetectionRadius, 2))
        {
            m_DetectionArea.setOutlineColor(sf::Color::Red);
            Shoot(GetPosition(), units[i]);
        }
        else 
        {
            m_DetectionArea.setOutlineColor(sf::Color::Cyan);
        }

    }
}

void Tower::Render(sf::RenderWindow& Renderer)
{
    if (IsPicked())
        Engine::GetSingleton()->DisplayTexture(("../Data/" + m_Name + ".png").c_str(), m_Position, DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot{0.5, 0.5}});
    else
        Engine::GetSingleton()->DisplayTexture(("../Data/" + m_Name + ".png").c_str(), m_Position, DisplayParameters{.Pivot{0.5, 0.5}});

    // -------debug-----------
    // m_DetectionArea.setFillColor(sf::Color::Transparent);
    // m_DetectionArea.setOutlineThickness(3.f);
    // m_DetectionArea.setPosition(m_Position - vec2(m_DetectionRadius, m_DetectionRadius));
    // Renderer.draw(m_DetectionArea);
}

void Tower::Shoot(vec2 StartingPosition, shared_ptr<Unit> Target)
{
    if (m_ShootingTimer <= 0)
    {
        m_Game.Shoot(StartingPosition, Target);
        m_ShootingTimer = 1.f;
    }
}

bool Tower::OnMouseButtonDown(int Button)
{
    if (IsCursorOnButton())
    {
        m_Picked = !m_Picked;
        return true;
    }

    return false;
}

bool Tower::IsCursorOnButton()const
{
    vec2i mousePos = Engine::GetSingleton()->GetMousePos();

    vec2 objectTopLeft = m_Position - m_Size / 2;
    vec2 objectBottomRight = m_Position + m_Size / 2;

    if (mousePos.x >= objectTopLeft.x &&  mousePos.x <= objectBottomRight.x)
    {
        if (mousePos.y >= objectTopLeft.y && mousePos.y <= objectBottomRight.y)
        {
            return true;
        }
    }

    return false;
}

bool Tower::IsPicked()const
{
    return m_Picked;
}

 int Tower::GetPrize()const
 {
    return m_Cost;
 }