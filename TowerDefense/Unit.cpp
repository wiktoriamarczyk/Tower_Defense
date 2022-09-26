#include "Unit.h"
#include "Engine.h"
#include "ToolTip.h"

Unit::Unit(vec2 Position)
{
    SetPosition(Position);
}

void Unit::Update(float DeltaTime)
{
    m_Animator.Update(DeltaTime);

    // sprawdzenie czy jednostka zostala usmiercona
    if (GetHP() <= 0)
    {
        auto OnAnimEnd = [this]()
        {
            SetLifeStatus(false);
        };

        m_Animator.StartAnimation(eTextureType::DEATH,false,OnAnimEnd);

        return;
    }

    // sprawdzenie czy jednostka zostala postrzelona
    if (GetDamageStatus())
    {
        auto OnAnimEnd = [this]()
        {
            m_Animator.StartAnimation(eTextureType::DEFAULT,true);
        };

        m_Animator.StartAnimation(eTextureType::HIT,false,OnAnimEnd,true);

        SetDamageStatus(false);
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
    m_Animator.DisplayTexture(Renderer, GetPosition() , DisplayParameters{.Pivot{0.5, 0.5}});

    // poziom HP
    sf::RectangleShape lifeBar(vec2(50.f, 15.f));
    lifeBar.setFillColor(sf::Color::Transparent);
    lifeBar.setOutlineColor(sf::Color::White);
    lifeBar.setOutlineThickness(1.f);
    lifeBar.setPosition(vec2(GetPosition().x-25.f, GetPosition().y - 70.f));

    sf::RectangleShape lifeBarFilling;

    float lifePercent = m_HP / m_MaxHP;
    lifeBarFilling.setSize(vec2(45.f * lifePercent, 10.f));

    lifeBarFilling.setFillColor(sf::Color::Red);
    lifeBarFilling.setOutlineColor(sf::Color::Red);
    lifeBarFilling.setOutlineThickness(1.f);
    lifeBarFilling.setPosition(vec2(GetPosition().x-25.f + 3.f, GetPosition().y - 68.f));

    Renderer.draw(lifeBar);

    if (GetHP() > 0)
        Renderer.draw(lifeBarFilling);


    sf::RectangleShape bounds = sf::RectangleShape(vec2(GetSize()));
    bounds.setFillColor(sf::Color::Transparent);
    bounds.setOutlineColor(sf::Color::Red);
    bounds.setOutlineThickness(1.f);
    bounds.setPosition(GetPosition()-GetSize()/2);

    //Renderer.draw(bounds);
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

bool Unit::FillToolTip(ToolTip& MyToolTip)const
{
    MyToolTip.AddToolTipLine("Speed: " + ToString(m_Speed) + " ");
    MyToolTip.AddToolTipLine("💨", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .ContinueLastLine = true});

    if (m_Resistances.FireValue > 0)
    {
        MyToolTip.AddToolTipLine("Fire Resist: " + ToString(m_Resistances.FireValue) + " ");
        MyToolTip.AddToolTipLine("🔥", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color(255, 95, 31), .ContinueLastLine = true});
    }

    if (m_Resistances.LightningValue > 0)
    {
        MyToolTip.AddToolTipLine("Lightning Resist: " + ToString(m_Resistances.LightningValue) + " ");
        MyToolTip.AddToolTipLine("⚡", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color::Yellow, .ContinueLastLine = true});
    }

    if (m_Resistances.IceValue > 0)
    {
        MyToolTip.AddToolTipLine("Ice Resist: " + ToString(m_Resistances.IceValue) + " ");
        MyToolTip.AddToolTipLine("❄", 15, DrawTextParameters{.FontName = "NotoEmoji-Regular", .FontColor = sf::Color(0,191,255), .ContinueLastLine = true});
    }

    return true;
}

void Unit::MoveTo(vector<vec2> TargetPositions)
{
    m_TargetPositions = TargetPositions;
}

void Unit::Initialize(const Definition& Def)
{
    m_Name = Def.GetStringValue("Name");

    string textureName = Def.GetStringValue("AnimFileName", "MissingTexture");
    m_Animator.AddTexture(eTextureType::DEFAULT, textureName);

    auto DefaultTexture = textureName;

    textureName = Def.GetStringValue("DAnimFileName", "MissingTexture");
    m_Animator.AddTexture(eTextureType::DEATH, textureName);

    textureName = Def.GetStringValue("HAnimFileName", "MissingTexture");
    m_Animator.AddTexture(eTextureType::HIT, textureName);

    m_MaxHP = Def.GetIntValue("HP");
    m_Speed = Def.GetFloatValue("Speed", 200.f);
    m_MoneyForKill = Def.GetFloatValue("Money");
    m_Resistances.FireValue = Def.GetFloatValue("FireResistance");
    m_Resistances.LightningValue = Def.GetFloatValue("LightningResistance");
    m_Resistances.IceValue = Def.GetFloatValue("IceResistance");

    m_HP = m_MaxHP;

    m_ID = Engine::GetSingleton()->GenerateTextureID(DefaultTexture);
    SetSize(Engine::GetSingleton()->GetTextureSize(m_ID));
    m_Animator.StartAnimation(eTextureType::DEFAULT,true);
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

void Unit::ChangeTexture(eTextureType Texture)
{
    m_Animator.StartAnimation(Texture,true);
    //if (m_CurrentTexture.first == Texture)
    //{
    //    return;
    //}

    //for (size_t i = 0; i < m_TextureNames.size(); ++i)
    //{
    //    if (m_TextureNames[i].first == Texture)
    //    {
    //        m_CurrentTexture = m_TextureNames[i];
    //        break;
    //    }
    //}
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

float Unit::GetMoneyForKill()const
{
    return m_MoneyForKill;
}

void Unit::SetDamageStatus(bool Info)
{
    m_IsHurt = Info;
}

void Unit::SetHP(int Value)
{
    m_HP = Value;
}