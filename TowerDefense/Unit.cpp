#include "Unit.h"
#include "Engine.h"

Unit::Unit(vec2 Position)
{
    m_Position = Position;
    m_Size = vec2i(CELL_SIZE, CELL_SIZE);
}

void Unit::Update(float DeltaTime)
{
    m_HurtTimer -= DeltaTime;

    if (GetHP() <= 0)
        m_IsAlive = false;

    if (GetDamageStatus())
    {
        m_IsHurt = false;
        m_HurtTimer = 1.f;
    }

    if (m_TargetPositions.empty())
    {
        return;
    }

    if (m_Position != m_TargetPositions[0])
    {
        // wektor kierunkowy v = [x2 - x1, y2 - y1]
        vec2 dir(m_TargetPositions[0] - m_Position);
        vec2 normalizedDir = dir.GetNormalized();
        vec2 shiftPerFrame = normalizedDir * m_Speed * DeltaTime; 

        // jesli odleglosc do naszego punktu docelowego jest mniejsza niz odleglosc jaka przebedziemy w jednej klatce, to od razu wyladuj u celu
        if (shiftPerFrame.GetLength() > dir.GetLength())
            m_Position = m_TargetPositions[0];
        else
            m_Position += shiftPerFrame;
    }
    else
    {
        m_TargetPositions.erase(m_TargetPositions.begin());
    }
}

void Unit::Render(sf::RenderWindow& Renderer)
{
    // debug 
    // vector<sf::Vertex> vertices;
    // for (auto el : m_TargetPositions)
    // {
    //     vertices.push_back(sf::Vertex((sf::Vector2f)el, sf::Color::Cyan));
    // }

    // przekazujemy .data(), gdy� funkcja draw() przyjmuje wska�nik na pierwszy element i liczb� element�w w CPojemniku
    // Renderer.draw(vertices.data(), vertices.size(), sf::LineStrip);

    // TEKSTURA
    if (m_HurtTimer >= 0)
        Engine::GetSingleton()->DisplayTexture(("../Data/" + m_Name).c_str(), GetPosition() + m_Size / 2, DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot{0.5, 0.5}});
    else 
        Engine::GetSingleton()->DisplayTexture(("../Data/" + m_Name).c_str(), GetPosition() + m_Size / 2, DisplayParameters{.Pivot{0.5, 0.5}});


    // POZIOM HP
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