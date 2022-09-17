#include "Tower.h"
#include "Engine.h"
#include "Unit.h"
#include "ToolTip.h"

Tower::Tower(InGameState& Game, vec2 Position) : m_Game(Game)
{
    SetSize(vec2i(CELL_SIZE,CELL_SIZE));
    SetPosition(vec2(((int(Position.x)/CELL_SIZE * CELL_SIZE) + CELL_SIZE / 2), (int(Position.y)/CELL_SIZE * CELL_SIZE) + CELL_SIZE / 2));
}

void Tower::DrawTowerOverlay(string TextureName, sf::RenderWindow& Renderer, bool IsBlocked)
{
    auto mousePos = (Engine::GetSingleton()->GetMousePos() / CELL_SIZE) * CELL_SIZE;

    if (IsBlocked)
        Engine::GetSingleton()->DisplayTexture((TextureName).c_str(), mousePos + vec2i(CELL_SIZE/2, CELL_SIZE/2), DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot = vec2(0.5,0.5), .DrawColor = sf::Color::Red});
    else
        Engine::GetSingleton()->DisplayTexture((TextureName).c_str(), mousePos + vec2i(CELL_SIZE/2, CELL_SIZE/2), DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot = vec2(0.5,0.5)});
}

void Tower::Update(float DeltaTime)
{
    vec2 objectTopLeft = GetPosition() - GetSize() / 2;
    vec2 objectBottomRight = GetPosition() + GetSize() / 2;

    m_ShootingTimer -= DeltaTime;

    // sprawdz, czy wroga jednostka znajduje sie w polu widzenia
    // wykorzystujac rownanie okregu o srodku w punkcie (a, b): (x-a)^2 + (y-b)^2 = r^2
    // srodek pola widzenia znajduje sie w centrum tekstury: m_Position

    vector<shared_ptr<Unit>> units = m_Game.GetObjects<Unit>();

    for (size_t i = 0; i < units.size(); ++i)
    {
        float checkIfInCircle = pow(units[i]->GetPosition().x - GetPosition().x, 2) + pow(units[i]->GetPosition().y - GetPosition().y, 2);

        if (checkIfInCircle <= pow(m_DetectionRadius, 2))
        {
            if (units[i]->GetHP() <= 0)
                continue;

            m_DetectionArea.setOutlineColor(sf::Color::Red * sf::Color(255,255,255,192));
            Shoot(units[i]);

            if (units[i]->GetHP() <= 0)
                m_UnitsKilled++;
        }
        else
        {
            m_DetectionArea.setOutlineColor(sf::Color::Black * sf::Color(255,255,255,192));
        }
    }
}

void Tower::Render(sf::RenderWindow& Renderer)
{
    if (IsPicked())
        Engine::GetSingleton()->DisplayTexture((m_TextureName).c_str(), GetPosition(), DisplayParameters{.DrawMode = eDrawMode::ADDITIVE, .Pivot{0.5, 0.5}});
    else
        Engine::GetSingleton()->DisplayTexture((m_TextureName).c_str(), GetPosition(), DisplayParameters{.Pivot{0.5, 0.5}});

    //-------debug-----------
    if (IsCursorOverObject())
    {
        m_DetectionArea.setFillColor(sf::Color::Black * sf::Color(255,255,255,128));
        m_DetectionArea.setOutlineThickness(3.f);
        m_DetectionArea.setPosition(m_Position - vec2(m_DetectionRadius, m_DetectionRadius));
        Renderer.draw(m_DetectionArea);
    }
}

bool Tower::OnMouseButtonDown(int Button)
{
    if (IsCursorOverObject())
    {
        m_Picked = !m_Picked;
        return true;
    }

    return false;
}

bool Tower::IsCursorOverObject()const
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

bool Tower::FillToolTip(ToolTip& MyToolTip)const
{
   MyToolTip.AddToolTipLine("Lvl: " + ToString(m_Lvl));

    if (m_Damage.FireValue > 0)
        MyToolTip.AddToolTipLine("Fire Dmg: "+ ToString(m_Damage.FireValue));
    if (m_Damage.LightningValue > 0)
       MyToolTip.AddToolTipLine("Lightning Dmg: "+ ToString(m_Damage.LightningValue));
    if (m_Damage.IceValue > 0)
        MyToolTip.AddToolTipLine("Ice Dmg: "+ ToString(m_Damage.IceValue));

    MyToolTip.AddToolTipLine("Radius: " + ToString(m_DetectionRadius));
    MyToolTip.AddToolTipLine("Speed: " + ToString(m_ShootInterval));
    MyToolTip.AddToolTipLine("Units killed: " + ToString(m_UnitsKilled));

    return true;
}

void Tower::Initialize(const Definition& Def)
{
    m_Name = Def.GetStringValue("Name");
    m_TextureName = Def.GetStringValue("FileName");
    m_Cost = Def.GetIntValue("Cost");
    m_Damage.FireValue = Def.GetFloatValue("FireDamage");
    m_Damage.LightningValue = Def.GetFloatValue("LightningDamage");
    m_Damage.IceValue = Def.GetFloatValue("IceDamage");
    m_DetectionRadius = Def.GetIntValue("DetectionRadius");
    m_ShootInterval = Def.GetFloatValue("ShootInterval");

    m_TextureSize = Engine::GetSingleton()->GetTextureSize((m_TextureName).c_str());
    m_DetectionArea.setRadius(m_DetectionRadius);
}

void Tower::Shoot(shared_ptr<Unit> Target)
{
    if (m_ShootingTimer <= 0)
    {
        m_Game.Shoot(GetSelf(), Target, m_Damage);
        m_ShootingTimer = m_ShootInterval;
    }
}

bool Tower::IsPicked()const
{
    return m_Picked;
}

 int Tower::GetPrize()const
 {
    return m_Cost;
 }

 int Tower::GetLvl()const
 {
    return m_Lvl;
 }

int Tower::GetKills()const
{
    return m_UnitsKilled;
}

void Tower::SetLvl(int Lvl)
{
    m_Lvl = Lvl;
}

void Tower::SetKills(int Value)
{
    m_UnitsKilled = Value;
}