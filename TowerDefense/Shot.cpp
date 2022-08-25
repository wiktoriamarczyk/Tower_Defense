#include "Shot.h"
#include "Engine.h"

Shot::Shot(vec2 StartingPosition, shared_ptr<Unit> Target)
{
    m_Target = Target;
    SetPosition(StartingPosition);
}

void Shot::Update(float DeltaTime)
{
    // wektor kierunkowy v = [x2 - x1, y2 - y1]
    vec2 dirVector = m_Target->GetPosition() - GetPosition();
    m_NormalizedDir = dirVector.GetNormalized();
    vec2 shiftPerFrame = m_NormalizedDir * m_Speed * DeltaTime; 

    SetPosition(GetPosition() + shiftPerFrame);

    vec2 objectTopLeft = m_Position - m_Size / 2;
    vec2 objectBottomRight = m_Position + m_Size / 2;

    vec2 unitTopLeft = m_Target->GetPosition() - m_Target->GetSize() / 2;
    vec2 unitBottomRight =  m_Target->GetPosition() + m_Target->GetSize() / 2;

    if (GetPosition().x >= unitTopLeft.x && GetPosition().x <= unitBottomRight.x
        && GetPosition().y >= unitTopLeft.y && GetPosition().y <= unitBottomRight.y)
        {
            m_Target->SetDamageStatus(true);
            m_Target->SetLifeCount(m_Target->GetLifeCount() - 1);
            m_IsAlive = false;
        }

    if (objectTopLeft.x < 0 || objectBottomRight.x >= SCREEN_WIDTH || objectBottomRight.y < 0 || objectTopLeft.y >= SCREEN_HEIGHT)
        m_IsAlive = false;
}

void Shot::Render(sf::RenderWindow& Renderer)
{
    //sf::RectangleShape shot(vec2(10.f, 10.f));
    //shot.setFillColor(sf::Color::Green);
    //shot.setPosition(GetPosition());

    Engine::GetSingleton()->DisplayTexture("Shot.png", m_Position, DisplayParameters{.DrawScale{0.1f, 0.1f}, .Pivot{0.5, 0.5}, .Rotation{m_NormalizedDir.GetAngleFromVec()}});

    //Renderer.draw(shot);

}