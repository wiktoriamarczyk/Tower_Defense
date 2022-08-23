#include "Shot.h"
#include "Engine.h"

Shot::Shot(vec2 Position, vec2 TargetPosition)
{
    m_Position = Position;
    // wektor kierunkowy v = [x2 - x1, y2 - y1]
    m_DirectionVec = TargetPosition - GetPosition();
}

void Shot::Update(float DeltaTime)
{
    vec2 normalizedDir = m_DirectionVec.GetNormalized();
    vec2 shiftPerFrame = normalizedDir * m_Speed * DeltaTime; 

    SetPosition(GetPosition() + shiftPerFrame);

    vec2 objectTopLeft = m_Position - m_Size / 2;
    vec2 objectBottomRight = m_Position + m_Size / 2;

    if (objectTopLeft.x < 0 || objectBottomRight.x >= SCREEN_WIDTH || objectBottomRight.y < 0 || objectTopLeft.y >= SCREEN_HEIGHT)
        m_IsAlive = false;
}

void Shot::Render(sf::RenderWindow& Renderer)
{
    //sf::RectangleShape shot(vec2(10.f, 10.f));
    //shot.setFillColor(sf::Color::Green);
    //shot.setPosition(GetPosition());

    Engine::GetSingleton()->DisplayTexture("Shot.png", m_Position, DisplayParameters{.DrawScale{0.1f, 0.1f}, .Pivot{0.5, 0.5}});

    //Renderer.draw(shot);

}