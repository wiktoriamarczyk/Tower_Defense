#include "Shot.h"
#include "Engine.h"

Shot::Shot(shared_ptr<Tower> Source, shared_ptr<Unit> Target, Damage DamageValue)
{
    m_Name = "/Textures/Shot.png";
    m_Target = Target;
    m_Source = Source;
    SetPosition(m_Source->GetPosition());
    SetSize(Engine::GetSingleton()->GetTextureSize(m_Name));
    m_Damage = DamageValue;
}

void Shot::Update(float DeltaTime)
{
    // wektor kierunkowy v = [x2 - x1, y2 - y1]
    vec2 dirVector = m_Target->GetPosition() - GetPosition();
    m_NormalizedDir = dirVector.GetNormalized();
    vec2 shiftPerFrame = m_NormalizedDir * m_Speed * DeltaTime;

    // przemieszczanie sie strzalu
    
    SetPosition(GetPosition() + shiftPerFrame);

    vec2 unitHitSize(20,20);

    // wymiary strzalu
    vec2 objectTopLeft = GetPosition() - GetSize() / 2;
    vec2 objectBottomRight = GetPosition() + GetSize() / 2;

    // wymiary jednostki
    vec2 unitTopLeft = m_Target->GetPosition() - unitHitSize / 2;
    vec2 unitBottomRight =  m_Target->GetPosition() + unitHitSize / 2;

    // trafienie w jendostke przez strzal wiezy 
    if (GetPosition().x >= unitTopLeft.x && GetPosition().x <= unitBottomRight.x
        && GetPosition().y >= unitTopLeft.y && GetPosition().y <= unitBottomRight.y)
        {
            m_Target->SetDamageStatus(true);
            m_Target->OnHit(m_Damage);

            if (m_Target->GetHP() <= 0)
            {
                 m_Source->SetKills(m_Source->GetKills() + 1);
                 m_Source->SetMoneyEarned(m_Target->GetMoneyForKill());
            }
            SetLifeStatus(false);
        }

    // jesli jednostka wyjdzie poza ekran, to umiera
    if (objectTopLeft.x < 0 || objectBottomRight.x >= SCREEN_WIDTH || objectBottomRight.y < 0 || objectTopLeft.y >= SCREEN_HEIGHT)
    {
        SetLifeStatus(false);
    }
}

void Shot::Render(sf::RenderWindow& Renderer)
{
    Engine::GetSingleton()->DisplayTexture(m_Name, GetPosition(), DisplayParameters{.DrawScale{0.1f, 0.1f}, .Pivot{0.5, 0.5}, .Rotation{m_NormalizedDir.GetAngleFromVec()}});
}