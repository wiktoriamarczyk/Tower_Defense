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
    /*vec2 Center_position = m_Position - m_Size / 2;
    sf::RectangleShape Rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    Rect.setOutlineThickness(4.f);
    Rect.setFillColor(sf::Color::Blue);
    Rect.setPosition(vec2(Center_position) + vec2(CELL_SIZE/2, CELL_SIZE/2));*/

    // debug 
    // vector<sf::Vertex> vertices;
    // for (auto el : m_TargetPositions)
    // {
    //     vertices.push_back(sf::Vertex((sf::Vector2f)el, sf::Color::Cyan));
    // }

    // przekazujemy .data(), gdy� funkcja draw() przyjmuje wska�nik na pierwszy element i liczb� element�w w CPojemniku
    // Renderer.draw(vertices.data(), vertices.size(), sf::LineStrip);

    /*Renderer.draw(Rect);*/

    Engine::GetSingleton()->DisplayTexture(("../Data/" + m_Name).c_str(), GetPosition() + m_Size / 2, DisplayParameters{.Pivot{0.5, 0.5}});
}

bool Unit::OnMouseButtonDown(int Button)
{
    return false;
}

void Unit::MoveTo(vector<vec2> TargetPositions)
{
    m_TargetPositions = TargetPositions;
}
