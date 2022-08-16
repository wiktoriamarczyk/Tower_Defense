#include "Unit.h"

Unit::Unit(vec2i Position, const string& UnitName)
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
        vec2 Dir(m_TargetPositions[0] - m_Position);
        vec2 Normalized_dir = Dir.GetNormalized();
        vec2 Shift_per_frame = Normalized_dir * m_Speed * DeltaTime; 

        // jesli odleglosc do naszego punktu docelowego jest mniejsza niz odleglosc jaka przebedziemy w jednej klatce, to od razu wyladuj u celu
        if (Shift_per_frame.GetLength() > Dir.GetLength())
            m_Position = m_TargetPositions[0];
        else
            m_Position += Shift_per_frame;
    }
    else
    {
        m_TargetPositions.erase(m_TargetPositions.begin());
    }
}

void Unit::Render(sf::RenderWindow& Renderer)
{
    vec2 Center_position = m_Position - m_Size / 2;
    sf::RectangleShape Rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    Rect.setOutlineThickness(4.f);
    Rect.setFillColor(sf::Color::Blue);
    Rect.setPosition(vec2(Center_position));
    Renderer.draw(Rect);
}

bool Unit::OnMouseButtonDown(int Button)
{
    return false;
}

void Unit::MoveTo(vector<vec2> TargetPositions)
{
    m_TargetPositions = TargetPositions;
}
