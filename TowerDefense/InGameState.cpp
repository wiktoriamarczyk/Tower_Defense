#include "InGameState.h"
#include "Engine.h"
#include "Button.h"
#include "Shot.h"

InGameState::InGameState() : GameState(eStateID::INGAME)
{
    if (!ReadGrid())
    {
        std::cout << "File could not be loaded!" << std::endl;
        return;
    }

    // inicjalizacja kursorow
    InitializeCursor(eCursorType::BUILD, "BuildCursor.png");
    InitializeCursor(eCursorType::SWORD, "SwordCursor.png");
    InitializeCursor(eCursorType::HOURGLASS, "HourglassCursor.png");
    InitializeCursor(eCursorType::SPELL, "SpellCursor.png");
    InitializeCursor(eCursorType::DEFAULT, "DefaultCursor.png");

    // zainicjalizowanie szukania sciezki dla jednostek
    if (!m_PathFinder.InitFinder(m_Grid))
        std::cout << "Path could not be found!" << std::endl;
}

InGameState::~InGameState() {}

void InGameState::OnEnter()
{
    m_GameOver = false;
    GameState::OnEnter();

    // ustawienie domyslnej predksci gry - 60 klatek na sekunde
    Engine::GetSingleton()->SetFramesPerSecond(60.f);

    // wyzerowanie obiektow gry
    m_AllGameObjects.clear();

    // ustawienie kursora
    SetCursor(eCursorType::DEFAULT);

    // ustawienie ilosci pieniedzy na domyslna wartosc
    SetMoneyAmount(1000.f);

    // wyzerowanie info o wiezach
    m_HoldTower = false;
    m_MoveTower = false;
    m_pTowerDef = nullptr;

    // wyzerowanie timerow
    m_TimeToNextUnitPhase = 20.f;
    Engine::GetSingleton()->StopAllTimers();

    // stworzenie obiektow gry
    CreateGameObjects();

    auto dragonPhaseFunc = [this]()
    {
        ChangeUnitPhase("Dragon");
    };

    Engine::GetSingleton()->StartTimer(40.f, true, dragonPhaseFunc, -40.f);

    auto basiliskPhaseFunc = [this]()
    {
        ChangeUnitPhase("Basilisk");

    };

    Engine::GetSingleton()->StartTimer(40.f, true, basiliskPhaseFunc, -20.f);

    auto unitSpawnerFunc = [this]()
    {
        CreateUnit(vec2i(60, -100), m_CurrentUnitPhase);
    };

    Engine::GetSingleton()->StartTimer(2.5f, true, unitSpawnerFunc, -2.5f);
}

bool InGameState::ReadGrid()
{
    fstream stream("../Data/Grid.txt");
    if (!stream)
    {
        std::cout << "File could not be found!" << std::endl;
        return false;
    }

    char tmp_char = 0;

    while (true)
    {
        if (stream.eof())
        {
            return true;
        }

        for (int i = 0; i < GRID_ROWS; ++i)
        {
            for (int j = 0; j < (GRID_COLS + 1) ; )
            {
                stream >> tmp_char;

                if (tmp_char != ',' && tmp_char != ';')
                {
                    m_Grid[i][j] = (eGridValue)tmp_char;
                    ++j;
                }

                if (tmp_char == ';')
                {
                    break;
                }
            }
        }
    }
}

void InGameState::OnMouseButtonDown(int Button)
{
    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;
    // numer w rzedzie i w kolumnie
    vec2i cell ((x / CELL_SIZE), (y / CELL_SIZE));
    // 0 - nie mozna polozyc obiektu, 1 - mozna polozyc obiekt, 2 - wybrany obiekt to wieza
    eGridValue gridState = m_Grid[cell.y % GRID_ROWS][cell.x % GRID_COLS];

    // klik na obiekt
    for (size_t i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->IsActive() && m_AllGameObjects[i]->OnMouseButtonDown(Button))
            return;
    }

    // odkliknij trzymane wieze, jesli nie wykonano zadnej interakcji z buttonem
   vector<shared_ptr<Tower>> towers = GetObjects<Tower>();
    for (size_t i = 0; i < towers.size(); ++i)
    {
       towers[i]->PickTower(false);
    }

    // wybudowanie wiezy po kliknieciu w wolne pole
    if (m_HoldTower)
    {
        if (gridState == eGridValue::FREE)
        {
            BuildTower(cell, m_pTowerDef);
            m_Money -= m_pTowerDef->GetFloatValue("Cost");
            m_HoldTower = false;
        }
        else
            m_HoldTower = false;
    }
}

void InGameState::OnKeyDown(sf::Keyboard::Key KeyCode)
{
    if (KeyCode == sf::Keyboard::Key::Escape)
    {
        m_AllGameObjects.clear();
        m_NextStateID = eStateID::MAINMENU;
    }

    // wyswietlanie siatki gry
    if (KeyCode == sf::Keyboard::Key::Numpad0)
    {
        _gridDebug = !_gridDebug;
    }
}

void InGameState::Update(float DeltaTime)
{
    // inicjalizacja tool tipow
    m_ToolTip->ClearToolTip();

    // licznik do nastepnej fali
    m_TimeToNextUnitPhase -= DeltaTime;

    if (m_TimeToNextUnitPhase <= 0)
        m_TimeToNextUnitPhase += 20.f;


    // sprawdz, czy wyswietlic tooltip obiektu
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->IsActive() && m_AllGameObjects[i]->IsCursorOverObject())
        {
            if(m_AllGameObjects[i]->FillToolTip(*m_ToolTip))
                break;
        }
    }

    // zmiana kursora
    shared_ptr<GameObject> object;

    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->IsActive() && m_AllGameObjects[i]->IsCursorOverObject())
        {
            object = m_AllGameObjects[i];
            break;
        }
    }

    if (object)
    {
        SetCursor(object->GetCursor());
        object = nullptr;
    }
    else
    {
        SetCursor(eCursorType::DEFAULT);
    }


    // update obiektow gry
    for (size_t i = 0; i < m_AllGameObjects.size();)
    {
        if (m_AllGameObjects[i]->IsActive())
            m_AllGameObjects[i]->Update(DeltaTime);

        if (m_AllGameObjects[i]->GetLifeStatus() == false)
            m_AllGameObjects.erase(m_AllGameObjects.begin() + i);

        else
            ++i;
    }
}

void InGameState::Render(sf::RenderWindow& Renderer)
{
    Renderer.clear(sf::Color::Black);

    // poprawne wyswietlanie obiektow roznych warstw na ekranie
    std::stable_sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), [](shared_ptr<GameObject> A, shared_ptr<GameObject> B){return A->GetGraphicLayer() < B->GetGraphicLayer();});

    // render aktywnych obiektow gry
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->IsActive())
            m_AllGameObjects[i]->Render(Renderer);
    }

    // render siatki
    int mouseX = Engine::GetSingleton()->GetMousePos().x;
    int mouseY = Engine::GetSingleton()->GetMousePos().y;
    int cellX = (mouseX / CELL_SIZE);
    int cellY = (mouseY / CELL_SIZE);
    eGridValue gridState = m_Grid[cellY % GRID_ROWS][cellX % GRID_COLS];

    // ustawienie koloru danego pola: biale - wolne, czerwone - zajete
    sf::Color gridColor;

    if (gridState == eGridValue::FREE)
       gridColor = sf::Color::White;
    else
       gridColor = sf::Color::Red;

    // render trzymanej wiezy
    if (m_HoldTower)
    {
        if (m_pTowerDef)
        {
            Tower::DrawTowerOverlay(m_pTowerDef->GetStringValue("FileName"), Renderer, gridState != eGridValue::FREE);
        }
    }

    // ----------------------debug--------------------------------
    if (_gridDebug)
    {

        for(int _x = 0; _x < SCREEN_WIDTH; _x += CELL_SIZE)
        {
            for(int _y = 0; _y < SCREEN_HEIGHT; _y += CELL_SIZE)
            {
                int _CellX = (_x / 64);
                int _CellY = (_y / 64);

                eGridValue _grid_state = m_Grid[_CellY % GRID_ROWS][_CellX % GRID_COLS];

                sf::Color _GridColor;
                if (_grid_state == eGridValue::FREE)
                _GridColor = sf::Color::White;
                else if (_grid_state == eGridValue::BLOCKED)
                _GridColor = sf::Color::Red;

                sf::RectangleShape Rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                Rect.setPosition(sf::Vector2f(_CellX * CELL_SIZE, _CellY * CELL_SIZE));
                Rect.setOutlineThickness(2.f);
                Rect.setOutlineColor(_GridColor);
                Rect.setFillColor(sf::Color::Transparent);
                Renderer.draw(Rect);
            }
        }
    }
    // -----------------------------------------------------------

    // wyswietlanie pozycji myszki na ekranie
    Engine::GetSingleton()->DrawText(ToString(mouseX), 8, vec2(1860, 1050));
    Engine::GetSingleton()->DrawText(ToString(mouseY), 8, vec2(1860, 1060));

    // rysowanie czcionki
    Engine::GetSingleton()->DrawText(ToString(GetMoneyAmount()), 8, vec2(1680, 1028));
    Engine::GetSingleton()->DrawText(ToString(m_TimeToNextUnitPhase), 8, vec2(1852, 970));

    if (Engine::GetSingleton()->GetFramesPerSecondValue() != 60.f)
    {
        Engine::GetSingleton()->DrawText("x", 8, vec2(1760, 1025));
        Engine::GetSingleton()->DrawText(ToString(60.f / Engine::GetSingleton()->GetFramesPerSecondValue()), 8, vec2(1775, 1025));
    }

    Renderer.display();
}

bool ObjectSort(shared_ptr<GameObject> A, shared_ptr<GameObject> B)
{
    if (A->GetGraphicLayer() < B->GetGraphicLayer())
        return true;

    if (A->GetGraphicLayer() > B->GetGraphicLayer())
        return false;

    auto pTowerA = dynamic_pointer_cast<Tower>(A);
    auto pTowerB = dynamic_pointer_cast<Tower>(B);

    if (pTowerA && pTowerB)
        return pTowerA->GetPosition().y < pTowerB->GetPosition().y;
    else
        return A->GetGraphicLayer() < B->GetGraphicLayer();

}
void InGameState::BuildTower(vec2 Cell, const Definition* pDef)
{
    auto pTower = make_shared<Tower>(*this, Engine::GetSingleton()->GetMousePos());
    if (!pDef)
        return;

    pTower->Initialize(*pDef);
    pTower->SetCursor(eCursorType::BUILD);
    m_AllGameObjects.push_back(pTower);

    // sortowanie wiezy po pozycji y, aby wieze znajdujace sie "blizej" gracza, byly widoczne na 1 planie
    sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), ObjectSort);

    // zablokowanie pola, na ktorym wybudowano wieze
    m_Grid[int(Cell.y) % GRID_ROWS][int(Cell.x) % GRID_COLS] = eGridValue::BLOCKED;
}

void InGameState::CreateUnit(vec2 Position, const string& UnitName)
{
    m_UnitPhaseIcon->InitializeUnitIcon(UnitName);

    const Definition* pDef = Engine::GetSingleton()->FindDefinition(string("/Definitions/" + UnitName + ".xml"));

    if (!pDef)
        return;

    auto pUnit = make_shared<Unit>(Position);
    pUnit->Initialize(*pDef);
    pUnit->SetCursor(eCursorType::SWORD);
    m_AllGameObjects.push_back(pUnit);

    // zainicjalizowanie sciezki dla jednostki
    vector<vec2> unitPath;

    if (!m_PathFinder.FindPath(vec2i(50, 50), vec2i(1480, 910), unitPath))
        std::cout << "Path could not be founded!" << std::endl;

    pUnit->MoveTo(unitPath);
}

shared_ptr<ParticleEmiter> InGameState::CreateParticles(vec2 Position, int ParticleCount, float ParticleScale, float MaxLifeTime, shared_ptr<GameObject> Target)
{
    shared_ptr<ParticleEmiter> pEmiter = make_shared<ParticleEmiter>("/Textures/Particle.png", ParticleCount, ParticleScale, MaxLifeTime);
    pEmiter->SetPosition(Position);

    if (Target)
        pEmiter->InitializeTarget(Target);

    m_AllGameObjects.push_back(pEmiter);
    return pEmiter;
}

void InGameState::DisplayTexture(const string& FileName, vec2i Position, DisplayParameters Param)
{
    Engine::GetSingleton()->DisplayTexture(FileName, Position, Param);
}

void InGameState::DestroyTextures()
{
    Engine::GetSingleton()->DestroyTextures();
}

void InGameState::Shoot(shared_ptr<Tower> Source, shared_ptr<Unit> Target, Damage DamageValue)
{
    shared_ptr<Shot> pShot = make_shared<Shot>(Source, Target, DamageValue);
    m_AllGameObjects.push_back(pShot);
    CreateParticles(Source->GetPosition(), 30, 0.25f, INFINITY, pShot);
}

void InGameState::EnableGroup(eUIGroup Group)
{
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->GetUIGroup() == Group)
            m_AllGameObjects[i]->SetActive(true);
    }
}

void InGameState::DisableGroup(eUIGroup Group)
{
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->GetUIGroup() == Group)
            m_AllGameObjects[i]->SetActive(false);
    }
}

void InGameState::InitializeCursor(eCursorType CursorType, string FilePath)
{
    auto cursorTexture = Engine::GetSingleton()->GetTexture("/Cursors/" + FilePath);
    auto pCursor = make_unique<pair<eCursorType, sf::Cursor>> ();
    pCursor->first = CursorType;
    pCursor->second.loadFromPixels(cursorTexture->GetSFMLTexture().copyToImage().getPixelsPtr(), sf::Vector2u(cursorTexture->GetSize()), sf::Vector2u(0, 0));
    m_AllCursors.push_back(std::move(pCursor));
}

void InGameState::ChangeUnitPhase(const string& Name)
 {
    m_CurrentUnitPhase = Name;
 }

void InGameState::SetCursor(eCursorType Cursor)
 {
    if (m_CurrentCursor == Cursor)
    {
        return;
    }

    for (size_t i = 0; i < m_AllCursors.size(); ++i)
    {
        if (m_AllCursors[i]->first == Cursor)
        {
            Engine::GetSingleton()->GetWindow().setMouseCursor(m_AllCursors[i]->second);
            m_CurrentCursor = Cursor;
            break;
        }
    }
 }


 void InGameState::CreateGameObjects()
 {
    const Definition* pTower1Def = Engine::GetSingleton()->FindDefinition("/Definitions/Tower1.xml");
    const Definition* pTower2Def = Engine::GetSingleton()->FindDefinition("/Definitions/Tower2.xml");
    const Definition* pTower3Def = Engine::GetSingleton()->FindDefinition("/Definitions/Tower3.xml");

    auto func = [this, pTower1Def]()
    {
        m_HoldTower = !m_HoldTower;

        if (m_HoldTower)
            m_pTowerDef = pTower1Def;
        else
            m_pTowerDef = nullptr;
    };

    auto func2 = [this, pTower2Def]()
    {
        m_HoldTower = !m_HoldTower;

        if (m_HoldTower)
            m_pTowerDef = pTower2Def;
        else
            m_pTowerDef = nullptr;
    };

    auto func3 = [this, pTower3Def]()
    {
        m_HoldTower = !m_HoldTower;

        if (m_HoldTower)
            m_pTowerDef = pTower3Def;
        else
            m_pTowerDef = nullptr;
    };

    auto func4 = [this]()
    {
            vector<shared_ptr<Tower>> towers = GetObjects<Tower>();

            for (size_t i = 0; i < towers.size(); ++i)
            {
                if(towers[i]->IsPicked())
                {
                    auto res = towers[i]->GetPrizeForUpgrade() / 2;
                    m_Money += towers[i]->GetPrizeForUpgrade() / 2;
                    int cellX = (towers[i]->GetPosition().x / CELL_SIZE);
                    int cellY = (towers[i]->GetPosition().y / CELL_SIZE);
                    m_Grid[cellY % GRID_ROWS][cellX % GRID_COLS] = eGridValue::FREE;
                    towers[i]->SetLifeStatus(false);
                }
            }
    };

    auto func5 = [this]()
    {
        float currentFramesPerSec = Engine::GetSingleton()->GetFramesPerSecondValue();

        if (currentFramesPerSec > 10)
            Engine::GetSingleton()->SetFramesPerSecond(currentFramesPerSec - 10.f);
    };

    auto func6 = [this]()
    {
        float currentFramesPerSec = Engine::GetSingleton()->GetFramesPerSecondValue();

        if (currentFramesPerSec < 100)
        Engine::GetSingleton()->SetFramesPerSecond(currentFramesPerSec + 10.f);
    };

    auto func7 = [this]()
    {

    };

    auto func8 = [this]()
    {
        vector<shared_ptr<Tower>> towers = GetObjects<Tower>();

            for (size_t i = 0; i < towers.size(); ++i)
            {
                if(towers[i]->IsPicked())
                {
                    m_Money -= towers[i]->GetPrizeForUpgrade();
                    towers[i]->LvlUp();
                }
            }
    };

    auto func9 = [this]()
    {
        DisableGroup(eUIGroup::SPELLS);
        EnableGroup(eUIGroup::TOWERS);
    };

    auto func10 = [this]()
    {
        DisableGroup(eUIGroup::TOWERS);
        EnableGroup(eUIGroup::SPELLS);
    };

    //----------wieze--------------------

    shared_ptr<TowerButton> tower1Button = make_shared<TowerButton>("/Buttons/Tower1.png", vec2(1650, 290), vec2(0, 0), func, true);
    tower1Button->SetDefinition(pTower1Def);
    tower1Button->SetUIGroup(eUIGroup::TOWERS);
    tower1Button->SetCursor(eCursorType::BUILD);
    m_AllGameObjects.push_back(tower1Button);

    shared_ptr<TowerButton> tower2Button = make_shared<TowerButton>("/Buttons/Tower2.png", vec2(1820, 285), vec2(0, 0), func2, true);
    tower2Button->SetDefinition(pTower2Def);
    tower2Button->SetUIGroup(eUIGroup::TOWERS);
    tower2Button->SetCursor(eCursorType::BUILD);
    m_AllGameObjects.push_back(tower2Button);

    shared_ptr<TowerButton> tower3Button = make_shared<TowerButton>("/Textures/Tower3Anim.xml", vec2(1675, 410), vec2(0, 0), func3, true);
    tower3Button->SetDefinition(pTower3Def);
    tower3Button->SetUIGroup(eUIGroup::TOWERS);
    tower3Button->SetCursor(eCursorType::BUILD);
    m_AllGameObjects.push_back(tower3Button);

    //-----------inne buttony-------------------

    shared_ptr<Button> towerListButton = make_shared<Button>("/Buttons/TowerList.png", vec2(1660, 800), vec2(0, 0), func9);
    towerListButton->SetCursor(eCursorType::BUILD);
    m_AllGameObjects.push_back(towerListButton);

    shared_ptr<Button> spellsBookButton = make_shared<Button>("/Buttons/SpellsBook.png", vec2(1750, 800), vec2(0, 0), func10);
    spellsBookButton->SetCursor(eCursorType::SPELL);
    m_AllGameObjects.push_back(spellsBookButton);

    shared_ptr<Button> prevButton = make_shared<Button>("/Buttons/PrevButton.png", vec2(1845, 800), vec2(0, 0), func7);
    m_AllGameObjects.push_back(prevButton);

    shared_ptr<Button> nextButton = make_shared<Button>("/Buttons/NextButton.png", vec2(1875, 800), vec2(0, 0), func7);
    m_AllGameObjects.push_back(nextButton);

    shared_ptr<Button> sellButton = make_shared<Button>("/Buttons/SellButton.png", vec2(1750, 930), vec2(0, 0), func4);
    sellButton->SetToolTipText({{"Sell tower"}, {"for half prize"}});
    sellButton->SetCursor(eCursorType::BUILD);
    m_AllGameObjects.push_back(sellButton);

    shared_ptr<Button> buttonUp = make_shared<Button>("/Buttons/ButtonUp.png", vec2(1750, 970), vec2(0, 0), func5);
    buttonUp->SetToolTipText({{"Speed up time"}});
    buttonUp->SetCursor(eCursorType::HOURGLASS);
    m_AllGameObjects.push_back(buttonUp);

    shared_ptr<Button> buttonDown = make_shared<Button>("/Buttons/ButtonDown.png", vec2(1750, 1000), vec2(0, 0), func6);
    buttonDown->SetToolTipText({{"Slow down time"}});
    buttonDown->SetCursor(eCursorType::HOURGLASS);
    m_AllGameObjects.push_back(buttonDown);

    shared_ptr<Button> lvlUpButton = make_shared<Button>("/Buttons/LvlUpButton.png", vec2(1840, 860), vec2(0, 0), func8);
    lvlUpButton->SetToolTipText({{"Upgrade the tower!"}});
    lvlUpButton->SetCursor(eCursorType::BUILD);
    m_AllGameObjects.push_back(lvlUpButton);

   //----------spelle--------------------

    shared_ptr<Button> moneySpell = make_shared<Button>("/Spells/AddMoneySpell.png", vec2(1680, 310), vec2(0, 0), func7);
    moneySpell->SetUIGroup(eUIGroup::SPELLS);
    moneySpell->SetCursor(eCursorType::SPELL);
    m_AllGameObjects.push_back(moneySpell);

    shared_ptr<Button> speedSpell = make_shared<Button>("/Spells/SpeedSpell.png", vec2(1820, 310), vec2(0, 0), func7);
    speedSpell->SetUIGroup(eUIGroup::SPELLS);
    speedSpell->SetCursor(eCursorType::SPELL);
    m_AllGameObjects.push_back(speedSpell);

    shared_ptr<Button> fireSpell = make_shared<Button>("/Spells/FireSpell.png", vec2(1680, 440), vec2(0, 0), func7);
    fireSpell->SetUIGroup(eUIGroup::SPELLS);
    fireSpell->SetCursor(eCursorType::SPELL);
    m_AllGameObjects.push_back(fireSpell);

    shared_ptr<Button> criticalSpell = make_shared<Button>("/Spells/CriticalShotSpell.png", vec2(1820, 440), vec2(0, 0), func7);
    criticalSpell->SetUIGroup(eUIGroup::SPELLS);
    criticalSpell->SetCursor(eCursorType::SPELL);
    m_AllGameObjects.push_back(criticalSpell);

    // wylaczenie renderowania i update'owania spelli na starcie
    DisableGroup(eUIGroup::SPELLS);

    //------------------------------

    shared_ptr<Image> backgroundImage = make_shared<Image>("/Images/GameBackground.png", vec2(0, 0), vec2(0, 0));
    backgroundImage->SetGraphicLayer(eGraphicLayer::BACKGROUND);
    m_AllGameObjects.push_back(backgroundImage);

    shared_ptr<Image> overlayImageMap = make_shared<Image>("/Images/OverlayMap.png", vec2(MAP_X, MAP_Y), vec2(0, 0));
    overlayImageMap->SetGraphicLayer(eGraphicLayer::FOREGROUND);
    m_AllGameObjects.push_back(overlayImageMap);

    shared_ptr<Image> overlayImage = make_shared<Image>("/Images/GameOverlay.png", vec2(0, 0), vec2(0, 0));
    overlayImage->SetGraphicLayer(eGraphicLayer::OVERLAY);
    m_AllGameObjects.push_back(overlayImage);


    shared_ptr<Image> unitPhaseIcon = make_shared<Image>("", vec2(1850, 930), vec2(0, 0));
    unitPhaseIcon->SetGraphicLayer(eGraphicLayer::UI);
    unitPhaseIcon->SetCursor(eCursorType::SWORD);
    m_UnitPhaseIcon = unitPhaseIcon;
    m_AllGameObjects.push_back(unitPhaseIcon);

    shared_ptr<Image> unitIconFrame = make_shared<Image>("/Images/UnitIconFrame.png", vec2(1850, 930), vec2(0, 0));
    unitIconFrame->SetGraphicLayer(eGraphicLayer::UI);
    m_AllGameObjects.push_back(unitIconFrame);

    shared_ptr<Image> moneyImage = make_shared<Image>("/Images/Money.png", vec2(1660, 930), vec2(0, 0));
    moneyImage->SetGraphicLayer(eGraphicLayer::UI);
    m_AllGameObjects.push_back(moneyImage);

    //------------------------------

    shared_ptr<ToolTip> myToolTip = make_shared<ToolTip>();
    myToolTip->SetGraphicLayer(eGraphicLayer::UI);
    m_ToolTip = myToolTip;
    m_AllGameObjects.push_back(myToolTip);
 }

float InGameState::GetMoneyAmount()const
{
    return m_Money;
}

void InGameState::SetMoneyAmount(float Value)
{
    m_Money = Value;
}
