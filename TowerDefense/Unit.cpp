#include "Unit.h"
#include "Engine.h"

Unit::Unit(vec2 Position)
{
    SetPosition(Position);
    //SetSize(vec2i(CELL_SIZE, CELL_SIZE));
}

void Unit::Update(float DeltaTime)
{
    m_HurtTimer -= DeltaTime;

    if (GetHP() <= 0)
    {
        m_DyingTimer -= DeltaTime;

        if (m_DyingTimer <= 0)
            SetLifeStatus(false);

        return;
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
        Engine::GetSingleton()->DisplayTexture(m_TextureName, GetPosition() , DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot{0.5, 0.5}});
    else
        Engine::GetSingleton()->DisplayTexture(m_TextureName, GetPosition() , DisplayParameters{.Pivot{0.5, 0.5}});

    // poziom HP
    sf::RectangleShape lifeBar(vec2(50.f, 15.f));
    lifeBar.setFillColor(sf::Color::Transparent);
    lifeBar.setOutlineColor(sf::Color::White);
    lifeBar.setOutlineThickness(1.f);
    lifeBar.setPosition(vec2(GetPosition().x, GetPosition().y - 70.f));

    sf::RectangleShape lifeBarFilling;

    float lifePercent = m_HP / m_MaxHP;
    lifeBarFilling.setSize(vec2(45.f * lifePercent, 10.f));

    lifeBarFilling.setFillColor(sf::Color::Red);
    lifeBarFilling.setOutlineColor(sf::Color::Red);
    lifeBarFilling.setOutlineThickness(1.f);
    lifeBarFilling.setPosition(vec2(GetPosition().x + 3.f, GetPosition().y - 68.f));

    Renderer.draw(lifeBar);

    if (GetHP() > 0)
        Renderer.draw(lifeBarFilling);
}

bool Unit::OnMouseButtonDown(int Button)
{
    return false;
}

bool Unit::IsCursorOverObject()const
{
    vec2i mousePos = Engine::GetSingleton()->GetMousePos();

    vec2 objectTopLeft = GetPosition() - GetSize() / 2;
    vec2 objectBottomRight = GetPosition() + GetSize() / 2;

    if (mousePos.x >= objectTopLeft.x &&  mousePos.x <= objectBottomRight.x)
    {
        if (mousePos.y >= objectTopLeft.y && mousePos.y <= objectBottomRight.y)
        {
            return true;
        }
    }

    return false;
}

vector<string> Unit::GetToolTip()const
{
     vector<string> tmp;

    tmp.push_back("Speed: " + ToString(m_Speed));

    if (m_Resistances.FireValue > 0)
        tmp.push_back("Fire Resist: "+ ToString(m_Resistances.FireValue));
    if (m_Resistances.LightningValue > 0)
        tmp.push_back("Lightning Resist: "+ ToString(m_Resistances.LightningValue));
    if (m_Resistances.IceValue > 0)
        tmp.push_back("Ice Resist: "+ ToString(m_Resistances.IceValue));

    return tmp;
}

void Unit::MoveTo(vector<vec2> TargetPositions)
{
    m_TargetPositions = TargetPositions;
}

void Unit::Initialize(const Definition& Def)
{
    m_Name = Def.GetStringValue("Name");
    m_TextureName = Def.GetStringValue("FileName", "MissingTexture");
    m_Speed = Def.GetFloatValue("Speed", 200.f);
    m_Resistances.FireValue = Def.GetFloatValue("FireResistance");
    m_Resistances.LightningValue = Def.GetFloatValue("LightningResistance");
    m_Resistances.IceValue = Def.GetFloatValue("IceResistance");

    m_MaxHP = Def.GetIntValue("HP");
    m_HP = m_MaxHP;

    SetSize(Engine::GetSingleton()->GetTextureSize(m_TextureName));
}

void Unit::OnHit(Damage DamageValue)
{
    float fireDamage = DamageValue.FireValue -  DamageValue.FireValue * m_Resistances.FireValue;
    fireDamage = max(fireDamage, 0.f);
    float lightningDamage = DamageValue.LightningValue - DamageValue.LightningValue * m_Resistances.LightningValue;
    lightningDamage = max(lightningDamage, 0.f);
    float iceDamage = DamageValue.IceValue - DamageValue.IceValue * m_Resistances.IceValue;
    iceDamage = max(iceDamage, 0.f);

    m_HP -= fireDamage + lightningDamage + iceDamage;
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