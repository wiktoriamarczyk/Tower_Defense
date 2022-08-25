#include "Unit.h"
#include "Engine.h"

Unit::Unit(vec2 Position, const string& UnitName)
{
    m_Position = Position;
    m_Name = UnitName;
    m_Size = vec2i(CELL_SIZE, CELL_SIZE);
}

void Unit::Update(float DeltaTime)
{
    if (m_TargetPositions.empty())
    {
        return;
    }

    m_HurtTimer -= DeltaTime;

    if (GetLifeCount() <= 0)
        m_IsAlive = false;

    if (GetDamageStatus())
    {
        m_IsHurt = false;
        m_HurtTimer = 1.f;
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

    if (GetLifeCount() > 1)
        lifeBarFilling.setSize(vec2(45.f, 10.f));
    else lifeBarFilling.setSize(vec2(20.f, 10.f));

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

 bool Unit::GetDamageStatus()const
 {
    return m_IsHurt;
 }
 
int Unit::GetLifeCount()const
{
    return m_LifeCount;
}

void Unit::SetDamageStatus(bool Info)
{
    m_IsHurt = Info;
}

void Unit::SetLifeCount(int Value)
{
    m_LifeCount = Value;
}