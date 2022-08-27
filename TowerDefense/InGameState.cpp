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

    m_CursorHand.loadFromSystem(sf::Cursor::Hand);
    m_CursorArrow.loadFromSystem(sf::Cursor::Arrow);

    if (!m_PathFinder.InitFinder(m_Grid))
        std::cout << "Path could not be found!" << std::endl;

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
            BuildTower(cell, m_ObjectName, m_TowerCost);
            m_Money -= m_TowerCost;
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

    // przyspieszanie/zwalnianie czasu gry
    if (KeyCode == sf::Keyboard::Key::Add)
    {
        float currentFramesPerSec = Engine::GetSingleton()->GetFramesPerSecondValue();

        if (currentFramesPerSec > 10)
            Engine::GetSingleton()->SetFramesPerSecond(currentFramesPerSec - 10.f);
        
        std::cout << Engine::GetSingleton()->GetFramesPerSecondValue() << std::endl;
    }

    if (KeyCode == sf::Keyboard::Key::Subtract)
    {
        float currentFramesPerSec = Engine::GetSingleton()->GetFramesPerSecondValue();
        Engine::GetSingleton()->SetFramesPerSecond(currentFramesPerSec + 10.f);

        std::cout << Engine::GetSingleton()->GetFramesPerSecondValue() << std::endl;
    }

}

void InGameState::Update(float DeltaTime)
{
    m_SpawningTimer -= DeltaTime;

    if (m_SpawningTimer <= 0)
    {
        vector<const char*> Units = { "Dragon.xml" , "Basilisk.xml" };
        auto SelectedUnit = rand() % (Units.size());

        CreateUnit(vec2i(60, -100), Units[SelectedUnit] );
        m_SpawningTimer = 3.f;
    }

   // zmiana kursora
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        none_of(m_AllGameObjects.begin(), m_AllGameObjects.end(), [this](shared_ptr<GameObject> o)
        {
            if (o->IsCursorOnButton())
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
    
        if (m_AllGameObjects[i]->GetStatus() == false)
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

    if (m_HoldTower)
        Tower::DrawTowerOverlay(m_ObjectName, Renderer, gridState != eGridValue::FREE);

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
    m_Font->DrawText(Renderer, 1, 60, 1058, ToString(m_Money).c_str());

    if (m_Money < (int)eTowerPrice::Tower1)
    {
        m_Font->DrawText(Renderer, 1, 1700, 388, ToString((int)eTowerPrice::Tower1).c_str(), sf::Color::Red);
    }
    else m_Font->DrawText(Renderer, 1, 1700, 388, ToString((int)eTowerPrice::Tower1).c_str());


    if (m_Money < (int)eTowerPrice::Tower2)
    {
        m_Font->DrawText(Renderer, 1, 1835, 388, ToString((int)eTowerPrice::Tower2).c_str(), sf::Color::Red);
    }
    else m_Font->DrawText(Renderer, 1, 1835, 388, ToString((int)eTowerPrice::Tower2).c_str());

    Renderer.display();
}

void InGameState::OnEnter()
{
    m_GameOver = false;
    GameState::OnEnter();
}

void InGameState::BuildTower(vec2 Cell, const string& TowerName, int Cost)
{
    auto pTower = make_shared<Tower>(*this, Engine::GetSingleton()->GetMousePos(), TowerName, Cost);
    m_AllGameObjects.push_back(pTower);

    // sortowanie wiezy po pozycji y, aby wieze znajdujace sie "blizej" gracza, byly widoczne na 1 planie
    sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), [](shared_ptr<GameObject> p1, shared_ptr<GameObject> p2) { return p1->GetPosition().y < p2->GetPosition().y;  });

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

    if(!m_PathFinder.FindPath(vec2i(50, 50), vec2i(1480, 580), unitPath))
        std::cout << "Path could not be founded!" << std::endl;

    pUnit->MoveTo(unitPath);
}

void InGameState::DestroyTextures()
{
    Engine::GetSingleton()->DestroyTextures();
}

void InGameState::DisplayTexture(const string& FileName, vec2i Position, DisplayParameters Param)
{
    Engine::GetSingleton()->DisplayTexture(("../Data/" + FileName).c_str(), Position, Param);
}

void InGameState::Shoot(vec2 StartingPosition, shared_ptr<Unit> Target)
{
    shared_ptr<Shot> pShot = make_shared<Shot>(StartingPosition, Target);
    m_AllGameObjects.push_back(pShot);
}

 void InGameState::CreateGameObjects()
 {
    auto func = [this]()
    {
        if (!m_HoldTower)
        {
            m_HoldTower = true;
            m_ObjectName = "Tower1";
            m_TowerCost = 100;
        }
        else if (m_HoldTower)
        {
            m_HoldTower = false;
        }
    };

    auto func2 = [this]()
    {
        if (!m_HoldTower)
        {
            m_HoldTower = true;
            m_ObjectName = "Tower2";
            m_TowerCost = 250;
        }
        else if (m_HoldTower)
        {
            m_HoldTower = false;
        }
    };

    auto func3 = [this]()
    {
        if (!m_HoldTower)
        {
            m_HoldTower = true;
            m_ObjectName = "Tower3";
            m_TowerCost = 50;
        }
        else if (m_HoldTower)
        {
            m_HoldTower = false;
        }
    };

    auto func4 = [this]()
    {
        m_ObjectName = "SellButton";

        vector<shared_ptr<Tower>> towers = GetObjects<Tower>();

        for (size_t i = 0; i < towers.size(); ++i)
        {
            if(towers[i]->IsPicked())
            {
                m_Money += towers[i]->GetPrize() / 2;
                int cellX = (towers[i]->GetPosition().x / CELL_SIZE);
                int cellY = (towers[i]->GetPosition().y / CELL_SIZE);
                m_Grid[cellY % GRID_ROWS][cellX % GRID_COLS] = eGridValue::FREE;
                towers[i]->SetStatus(false);
            }
        }
    };


    vec2i buttonSize = Engine::GetSingleton()->GetTextureSize("../Data/Tower1.png");
    shared_ptr<Button> tower1Button = make_shared<Button>("Tower1.png", vec2i(1660, 290), buttonSize, func);
    m_AllGameObjects.push_back(tower1Button);

    buttonSize = Engine::GetSingleton()->GetTextureSize("../Data/Tower2.png");
    shared_ptr<Button> tower2Button = make_shared<Button>("Tower2.png", vec2i(1820, 280), buttonSize, func2);
    m_AllGameObjects.push_back(tower2Button);

    buttonSize = Engine::GetSingleton()->GetTextureSize("../Data/Tower3.png");
    shared_ptr<Button> tower3Button = make_shared<Button>("Tower3.png", vec2i(1660, 410), buttonSize, func3);
    m_AllGameObjects.push_back(tower3Button);

    buttonSize = Engine::GetSingleton()->GetTextureSize("../Data/SellButton.png");
    shared_ptr<Button> TrashCanButton = make_shared<Button>("SellButton.png", vec2i(1680, 825), buttonSize, func4);
    m_AllGameObjects.push_back(TrashCanButton);

    shared_ptr<Image> backgroundImage = make_shared<Image>("Background", vec2(0, 0), vec2(0, 0)); 
    backgroundImage->SetGraphicLayer(eGraphicLayer::BACKGROUND);
    m_AllGameObjects.push_back(backgroundImage);

    shared_ptr<Image> overlayImage = make_shared<Image>("Overlay", vec2(0, 0), vec2(0, 0)); 
    overlayImage->SetGraphicLayer(eGraphicLayer::FOREGROUND);
    m_AllGameObjects.push_back(overlayImage);
 }