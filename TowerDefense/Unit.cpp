#include "Unit.h"
#include "Engine.h"

Unit::Unit(vec2 Position)
{
    SetPosition(Position);
    SetSize(vec2i(CELL_SIZE, CELL_SIZE));
}

void Unit::Update(float DeltaTime)
{
    m_HurtTimer -= DeltaTime;

    if (GetHP() <= 0)
    {
        SetLifeStatus(false);
    }

    if (GetDamageStatus())
    {
        m_IsHurt = false;
        m_HurtTimer = 1.f;
    }

    // podazanie wyznaczona w InGameState sciezka
    if (m_TargetPositions.empty())
    {
        SetLifeStatus(false);
        return;
    }

    if (GetPosition() != m_TargetPositions[0])
    {
        // wektor kierunkowy v = [x2 - x1, y2 - y1]
        vec2 dir(m_TargetPositions[0] - GetPosition());
        vec2 normalizedDir = dir.GetNormalized();
        vec2 shiftPerFrame = normalizedDir * m_Speed * DeltaTime; 

        // jesli odleglosc do naszego punktu docelowego jest mniejsza niz odleglosc jaka przebedziemy w jednej klatce, to od razu wyladuj u celu
        if (shiftPerFrame.GetLength() > dir.GetLength())
            SetPosition(m_TargetPositions[0]);
        else
            SetPosition(GetPosition() + shiftPerFrame);
    }
    else
    {
        m_TargetPositions.erase(m_TargetPositions.begin());
    }
}

void Unit::Render(sf::RenderWindow& Renderer)
{
    // wyliczamy mnoznik, ktory przeskalowuje pozycje z ekranu na mini mape o rozmiarze (200,200)
    vec2 worldToMapRatio = vec2(MAP_WIDTH, MAP_HEIGHT) / vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
    // wyliczamy pozycje na mini mapie, dodajac do pozycji mapy, przeskalowana pozycje jednostki 
    vec2 mapPosition = vec2(MAP_X, MAP_Y) + (GetPosition() * worldToMapRatio);

    // renderowanie pozycji jednostki na mini mapie
    sf::RectangleShape unitOnMap(vec2(4.f, 4.f));
    unitOnMap.setFillColor(sf::Color::Red);
    unitOnMap.setPosition(mapPosition - vec2(2.f, 2.f));
    Renderer.draw(unitOnMap);

    // tekstura
    if (m_HurtTimer >= 0)
        Engine::GetSingleton()->DisplayTexture(m_Name, GetPosition() + GetSize() / 2, DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot{0.5, 0.5}});
    else 
        Engine::GetSingleton()->DisplayTexture(m_Name, GetPosition() + GetSize() / 2, DisplayParameters{.Pivot{0.5, 0.5}});

    // poziom HP
    sf::RectangleShape lifeBar(vec2(50.f, 15.f));
    lifeBar.setFillColor(sf::Color::Transparent);
    lifeBar.setOutlineColor(sf::Color::White);
    lifeBar.setOutlineThickness(1.f);
    lifeBar.setPosition(vec2(GetPosition().x, GetPosition().y - 30.f));

    sf::RectangleShape lifeBarFilling;

    float lifePercent = m_HP / m_MaxHP;
    lifeBarFilling.setSize(vec2(45.f * lifePercent, 10.f));

    lifeBarFilling.setFillColor(sf::Color::Red);
    lifeBarFilling.setOutlineColor(sf::Color::Red);
    lifeBarFilling.setOutlineThickness(1.f);
    lifeBarFilling.setPosition(vec2(GetPosition().x + 3.f, GetPosition().y - 28.f));

    Renderer.draw(lifeBar);
    Renderer.draw(lifeBarFilling);
}

bool Unit::OnMouseButtonDown(int Button)
{
    return false;
}

void Unit::MoveTo(vector<vec2> TargetPositions)
{
    m_TargetPositions = TargetPositions;
}

void Unit::Initialize(const Definition& Def)
{
    m_Name = Def.GetStringValue("Name", "MissingTexture");
    m_Speed = Def.GetFloatValue("Speed", 200.f);
    m_MaxHP = Def.GetFloatValue("HP", 2);
    m_HP = m_MaxHP;
}

 bool Unit::GetDamageStatus()const
 {
    return m_IsHurt;
 }
 
int Unit::GetHP()const
{
    return m_HP;
}

int Unit::GetMaxHP() const
{
    return m_MaxHP;
}

void Unit::SetDamageStatus(bool Info)
{
    m_IsHurt = Info;
}

void Unit::SetHP(int Value)
{
    m_HP = Value;
}