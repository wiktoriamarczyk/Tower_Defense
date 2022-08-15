#include "Unit.h"

Unit::Unit(vec2i Position, const string& UnitName)
{
    m_Position = Position;
    m_Name = UnitName;
}

void Unit::Update(float DeltaTime)
{
    if (m_Position != m_TargetPosition)
    {
        // wektor kierunkowy v = [x2 - x1, y2 - y1]
        vec2 Dir(m_TargetPosition - m_Position);
        vec2 Normalized_dir = Dir.GetNormalized();
        vec2 Shift_per_frame = Normalized_dir * m_Speed * DeltaTime; 

        if (Shift_per_frame.GetLength() > Dir.GetLength())
            m_Position = m_TargetPosition;
        else
            m_Position += Shift_per_frame;
    }
}

void Unit::Render(sf::RenderWindow& Renderer)
{
    sf::RectangleShape Rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    Rect.setOutlineThickness(4.f);
    Rect.setFillColor(sf::Color::Blue);
    Rect.setPosition(vec2(m_Position));
    Renderer.draw(Rect);
}

bool Unit::OnMouseButtonDown(int Button)
{
    return false;
}

void Unit::MoveTo(vec2 TargetPosition)
{
    m_TargetPosition = TargetPosition;
}
