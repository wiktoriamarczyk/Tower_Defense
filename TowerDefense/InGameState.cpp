#include "InGameState.h"
#include "Engine.h"
#include "Tower.h"
#include "Button.h"
#include "Image.h"
#include "Shot.h"

InGameState::InGameState(shared_ptr<Font> MyFont) : GameState(eStateID::INGAME)
{
    m_Font = MyFont;

    if (!ReadGrid())
    {
        std::cout << "File could not be loaded!" << std::endl;
        return;
    }
    
    // inicjalizacja kursorow
    auto cursorDefault = Engine::GetSingleton()->GetTexture("CursorDefault.png");
    auto cursorBuild = Engine::GetSingleton()->GetTexture("CursorBuild.png");

    if (cursorDefault && cursorBuild)
    {
        m_CursorArrow.loadFromPixels(cursorDefault->GetSFMLTexture().copyToImage().getPixelsPtr(), sf::Vector2u(cursorDefault->GetSize()), sf::Vector2u(0,0));
        m_CursorHand.loadFromPixels(cursorBuild->GetSFMLTexture().copyToImage().getPixelsPtr(), sf::Vector2u(cursorBuild->GetSize()), sf::Vector2u(0,0));
    }
    else
    {
        m_CursorArrow.loadFromSystem(sf::Cursor::Arrow);
        m_CursorHand.loadFromSystem(sf::Cursor::Hand);
    }

    // zainicjalizowanie szukania sciezki dla jednostek
    if (!m_PathFinder.InitFinder(m_Grid))
        std::cout << "Path could not be found!" << std::endl;


    // stworzenie obiektow gry
    CreateGameObjects();
}

InGameState::~InGameState() {}

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

    for (int i = 0; i < m_AllGameObjects.size(); ++i) 
    {
        if (m_AllGameObjects[i]->OnMouseButtonDown(Button))
            return;
    }

    if (m_HoldTower)
    {
        if (gridState == eGridValue::FREE)
        {
            BuildTower(cell, m_pTowerDef);
            m_Money -= m_pTowerDef->GetFloatValue("Cost");
            m_HoldTower = false;
        }
    }
}

void InGameState::OnKeyDown(sf::Keyboard::Key KeyCode)
{
    if (KeyCode == sf::Keyboard::Key::Escape)
    {
        m_AllGameObjects.clear();
        //m_NextStateID = eStateID::MAINMENU;
        Engine::GetSingleton()->ExitGame();
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
    m_ToolTip->InitializeToolTipText({});

    // ustalanie aktualnej fazy spawnowania jednostki
    m_SpawningTimer -= DeltaTime;
    m_UnitPhaseTimer -= DeltaTime;
    m_TimeToNextUnitPhase -= DeltaTime;

    if (m_TimeToNextUnitPhase <= 0)
        m_TimeToNextUnitPhase = 20.f;

    if (m_SpawningTimer <= 0)
    {
        //vector<const char*> Units = { "Dragon.xml" , "Basilisk.xml" };
        //auto SelectedUnit = rand() % (Units.size());

        if (m_UnitPhaseTimer <= 0)
            m_UnitPhaseTimer = 40.f;

        else if (m_UnitPhaseTimer >= 22.f)
            ChangeUnitPhase("Dragon");
        else 
            ChangeUnitPhase("Basilisk");

        m_SpawningTimer = 3.f;
    }

    // sprawdz czy wyswietlic tooltip obiektu
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->IsCursorOverObject())
        {
            vector<string> TooltipData = m_AllGameObjects[i]->GetToolTip();

            if(!TooltipData.empty())
            {
                m_ToolTip->InitializeToolTipText(TooltipData);
                break;
            }
        }
    }

   // zmiana kursora
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        none_of(m_AllGameObjects.begin(), m_AllGameObjects.end(), [this](shared_ptr<GameObject> o)
        {
            if (o->IsCursorOverObject())
            {
                Engine::GetSingleton()->GetWindow().setMouseCursor(m_CursorHand);
                return true;
            }
            else
            {
                Engine::GetSingleton()->GetWindow().setMouseCursor(m_CursorArrow);
                return false;
            }
        });
    }

    // update obiektow gry
    for (int i = 0; i < m_AllGameObjects.size();)
    {
        m_AllGameObjects[i]->Update(DeltaTime);
    
        if (m_AllGameObjects[i]->GetLifeStatus() == false)
        {
            m_AllGameObjects.erase(m_AllGameObjects.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void InGameState::Render(sf::RenderWindow& Renderer)
{
    Renderer.clear(sf::Color::Black);

    // poprawne wyswietlanie obiektow roznych warstw na ekranie
    std::stable_sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), [](shared_ptr<GameObject> A, shared_ptr<GameObject> B){return A->GetGraphicLayer() < B->GetGraphicLayer();});

    // render obiektow gry
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        m_AllGameObjects[i]->Render(Renderer);
    }

    // render siatki
    int mouseX = Engine::GetSingleton()->GetMousePos().x;
    int mouseY = Engine::GetSingleton()->GetMousePos().y;
    int cellX = (mouseX / CELL_SIZE);
    int cellY = (mouseY / CELL_SIZE);
    eGridValue gridState = m_Grid[cellY % GRID_ROWS][cellX % GRID_COLS];

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
    m_Font->DrawText(Renderer, 1, 1860, 1050, ToString(mouseX).c_str());
    m_Font->DrawText(Renderer, 1, 1860, 1060, ToString(mouseY).c_str());

    // rysowanie czcionki
    m_Font->DrawText(Renderer, 1, 1680, 1028, ToString(m_Money).c_str());
    m_Font->DrawText(Renderer, 1, 1852, 970, ToString(m_TimeToNextUnitPhase).c_str());

    if (Engine::GetSingleton()->GetFramesPerSecondValue() != 60.f)
    {
        m_Font->DrawText(Renderer, 1, 1760, 1028, "x");
        m_Font->DrawText(Renderer, 1, 1775, 1028, ToString(60.f / Engine::GetSingleton()->GetFramesPerSecondValue()).c_str());
    }

    Renderer.display();
}

void InGameState::OnEnter()
{
    m_GameOver = false;
    GameState::OnEnter();
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

    m_AllGameObjects.push_back(pTower);

    // sortowanie wiezy po pozycji y, aby wieze znajdujace sie "blizej" gracza, byly widoczne na 1 planie
    sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), ObjectSort);

    m_Grid[int(Cell.y) % GRID_ROWS][int(Cell.x) % GRID_COLS] = eGridValue::BLOCKED;
}

void InGameState::CreateUnit(vec2 Position, const string& UnitName)
{
    const Definition* pDef = Engine::GetSingleton()->FindDefinition(UnitName);

    if (!pDef)
        return;
     
    auto pUnit = make_shared<Unit>(Position);
    pUnit->Initialize(*pDef);
    m_AllGameObjects.push_back(pUnit);
    
    //pUnit->MoveTo(vector<vec2>{{60, 580}, {570, 580}, {570, 310}, {970, 310}, {970, 580}, {1480,580}, {1480, 1200}});
    vector<vec2> unitPath;

    if(!m_PathFinder.FindPath(vec2i(50, 50), vec2i(1480, 910), unitPath))
        std::cout << "Path could not be founded!" << std::endl;

    pUnit->MoveTo(unitPath);
}

void InGameState::DestroyTextures()
{
    Engine::GetSingleton()->DestroyTextures();
}

void InGameState::DisplayTexture(const string& FileName, vec2i Position, DisplayParameters Param)
{
    Engine::GetSingleton()->DisplayTexture(FileName, Position, Param);
}

void InGameState::Shoot(vec2 StartingPosition, shared_ptr<Unit> Target)
{
    shared_ptr<Shot> pShot = make_shared<Shot>(StartingPosition, Target);
    m_AllGameObjects.push_back(pShot);
}

 void InGameState::CreateGameObjects()
 {
    const Definition* pTower1Def = Engine::GetSingleton()->FindDefinition("Tower1.xml");
    const Definition* pTower2Def = Engine::GetSingleton()->FindDefinition("Tower2.xml");
    const Definition* pTower3Def = Engine::GetSingleton()->FindDefinition("Tower3.xml");

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
                    m_Money += towers[i]->GetPrize() / 2;
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
                    m_Money -= towers[i]->GetPrize() * 1.5f;
                    towers[i]->SetLvl(towers[i]->GetLvl() + 1);
                }   
            }
    };

    //------------------------------

    vec2i buttonSize = Engine::GetSingleton()->GetTextureSize("Tower1.png");
    shared_ptr<TowerButton> tower1Button = make_shared<TowerButton>("Tower1.png", vec2(1650, 290), buttonSize, func, true);
    tower1Button->SetDefinition(pTower1Def);
    m_AllGameObjects.push_back(tower1Button);

    buttonSize = Engine::GetSingleton()->GetTextureSize("Tower2.png");
    shared_ptr<TowerButton> tower2Button = make_shared<TowerButton>("Tower2.png", vec2(1820, 285), buttonSize, func2, true);
    tower2Button->SetDefinition(pTower2Def);
    m_AllGameObjects.push_back(tower2Button);

    buttonSize = Engine::GetSingleton()->GetTextureSize("Tower3Anim.xml");
    shared_ptr<TowerButton> tower3Button = make_shared<TowerButton>("Tower3Anim.xml", vec2(1675, 410), buttonSize, func3, true);
    tower3Button->SetDefinition(pTower3Def);
    m_AllGameObjects.push_back(tower3Button);

    //------------------------------

    buttonSize = Engine::GetSingleton()->GetTextureSize("TowerList.png");
    shared_ptr<Button> towerListButton = make_shared<Button>("TowerList.png", vec2(1660, 800), buttonSize, func7);
    m_AllGameObjects.push_back(towerListButton);

    buttonSize = Engine::GetSingleton()->GetTextureSize("SpellsBook.png");
    shared_ptr<Button> spellsBookButton = make_shared<Button>("SpellsBook.png", vec2(1750, 800), buttonSize, func7);
    m_AllGameObjects.push_back(spellsBookButton);

    buttonSize = Engine::GetSingleton()->GetTextureSize("PrevButton.png");
    shared_ptr<Button> prevButton = make_shared<Button>("PrevButton.png", vec2(1845, 800), buttonSize, func7);
    m_AllGameObjects.push_back(prevButton);

    buttonSize = Engine::GetSingleton()->GetTextureSize("NextButton.png");
    shared_ptr<Button> nextButton = make_shared<Button>("NextButton.png", vec2(1875, 800), buttonSize, func7);
    m_AllGameObjects.push_back(nextButton);



    buttonSize = Engine::GetSingleton()->GetTextureSize("SellButton.png");
    shared_ptr<Button> sellButton = make_shared<Button>("SellButton.png", vec2(1750, 930), buttonSize, func4);
    sellButton->SetToolTipText({"sell tower", "for half prize"});
    m_AllGameObjects.push_back(sellButton);

    buttonSize = Engine::GetSingleton()->GetTextureSize("ButtonUp.png");
    shared_ptr<Button> buttonUp = make_shared<Button>("ButtonUp.png", vec2(1750, 970), buttonSize, func5);
    buttonUp->SetToolTipText({"Speed up time"});
    m_AllGameObjects.push_back(buttonUp);

    buttonSize = Engine::GetSingleton()->GetTextureSize("ButtonDown.png");
    shared_ptr<Button> buttonDown = make_shared<Button>("ButtonDown.png", vec2(1750, 1000), buttonSize, func6);
    buttonDown->SetToolTipText({"Slow down time"});
    m_AllGameObjects.push_back(buttonDown);

    buttonSize = Engine::GetSingleton()->GetTextureSize("LvlUpButton.png");
    shared_ptr<Button> lvlUpButton = make_shared<Button>("LvlUpButton.png", vec2(1840, 860), buttonSize, func8);
    lvlUpButton->SetToolTipText({"Upgrade the tower!"});
    m_AllGameObjects.push_back(lvlUpButton);

    //------------------------------

    shared_ptr<Image> backgroundImage = make_shared<Image>("Background.png", vec2(0, 0), vec2(0, 0)); 
    backgroundImage->SetGraphicLayer(eGraphicLayer::BACKGROUND);
    m_AllGameObjects.push_back(backgroundImage);

    shared_ptr<Image> overlayImageMap = make_shared<Image>("OverlayMap.png", vec2(MAP_X, MAP_Y), vec2(0, 0)); 
    overlayImageMap->SetGraphicLayer(eGraphicLayer::FOREGROUND);
    m_AllGameObjects.push_back(overlayImageMap);

    shared_ptr<Image> overlayImage = make_shared<Image>("Overlay.png", vec2(0, 0), vec2(0, 0)); 
    overlayImage->SetGraphicLayer(eGraphicLayer::OVERLAY);
    m_AllGameObjects.push_back(overlayImage);


    shared_ptr<Image> unitPhaseIcon = make_shared<Image>("", vec2(1850, 930), vec2(0, 0));
    unitPhaseIcon->SetGraphicLayer(eGraphicLayer::UI);
    m_UnitPhaseIcon = unitPhaseIcon;
    m_AllGameObjects.push_back(unitPhaseIcon);

    shared_ptr<Image> unitIconFrame = make_shared<Image>("UnitIconFrame.png", vec2(1850, 930), vec2(0, 0));
    unitIconFrame->SetGraphicLayer(eGraphicLayer::UI);
    m_AllGameObjects.push_back(unitIconFrame);

    shared_ptr<Image> moneyImage = make_shared<Image>("Money.png", vec2(1660, 930), vec2(0, 0));
    moneyImage->SetGraphicLayer(eGraphicLayer::UI);
    m_AllGameObjects.push_back(moneyImage);

    //------------------------------

    shared_ptr<ToolTip> myToolTip = make_shared<ToolTip>(m_Font);
    myToolTip->SetGraphicLayer(eGraphicLayer::UI);
    m_ToolTip = myToolTip;
    m_AllGameObjects.push_back(myToolTip);
 }

 void InGameState::ChangeUnitPhase(const string& Name)
 {
    m_UnitPhaseIcon->Initialize(Name);
    CreateUnit(vec2i(60, -100), string(Name + ".xml"));
 }