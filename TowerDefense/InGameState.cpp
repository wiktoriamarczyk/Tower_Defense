#include "InGameState.h"
#include "Engine.h"
#include "Tower.h"
#include "Button.h"


InGameState::InGameState(shared_ptr<Font> MyFont) : GameState(eStateID::INGAME)
{
    if (!ReadGrid())
    {
        std::cout << "File could not be loaded!" << std::endl;
        return;
    }

    m_Font = MyFont;

    m_CursorHand.loadFromSystem(sf::Cursor::Hand);
    m_CursorArrow.loadFromSystem(sf::Cursor::Arrow);

    auto func = [this]()
    {
        if (!m_HoldTower)
        {
            m_HoldTower = true;
            m_TowerName = "Tower1";
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
            m_TowerName = "Tower2";
            m_TowerCost = 250;
        }
        else if (m_HoldTower)
        {
            m_HoldTower = false;
        }
    };

    vec2i ButtonSize = Engine::GetSingleton()->GetTextureSize("../Data/Tower1.png");
    shared_ptr<Button> Tower1Button = make_shared<Button>("Tower1.png", vec2i(1660, 290), ButtonSize, func);
    m_AllGameObjects.push_back(Tower1Button);

    ButtonSize = Engine::GetSingleton()->GetTextureSize("../Data/Tower2.png");
    shared_ptr<Button> Tower2Button = make_shared<Button>("Tower2.png", vec2i(1820, 280), ButtonSize, func2);
    m_AllGameObjects.push_back(Tower2Button);

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
    vec2i Cell ((x / 64), (y / 64));
    // 0 - nie mozna polozyc obiektu, 1 - mozna polozyc obiekt, 2 - wybrany obiekt to wieza
    eGridValue grid_state = m_Grid[Cell.y % GRID_ROWS][Cell.x % GRID_COLS];

    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->OnMouseButtonDown(Button))
            return;
    }

    if (m_HoldTower)
    {
        if (grid_state == eGridValue::FREE)
        {
            BuildTower(Cell, m_TowerName);
            m_Money -= m_TowerCost;
        }

        m_HoldTower = false;
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

    if ((_gridDebug == false) && (KeyCode == sf::Keyboard::Key::Numpad0) )
    {
        _gridDebug = true;
    }

    else if ( (_gridDebug == true) && (KeyCode == sf::Keyboard::Key::Numpad0))
    {
        _gridDebug = false;
    }

}

void InGameState::Update(float DeltaTime)
{
    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;
    int CellX = (x / 64);
    int CellY = (y / 64);
    eGridValue grid_state = m_Grid[CellY % 17][CellX % 30];

   // ZMIANA KURSORA
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

    // UPDATE OBIEKTOW
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

    int MouseX = Engine::GetSingleton()->GetMousePos().x;
    int MouseY = Engine::GetSingleton()->GetMousePos().y;
    int CellX = (MouseX / 64);
    int CellY = (MouseY / 64);
    eGridValue grid_state = m_Grid[CellY % GRID_ROWS][CellX % GRID_COLS];

    // RENDER MAPY
    DisplayTexture("Background.png", vec2i(0,0), vec2i(1668, SCREEN_HEIGHT));

    sf::Color GridColor;

    if (grid_state == eGridValue::FREE)
       GridColor = sf::Color::White;
    else if (grid_state == eGridValue::BLOCKED)
       GridColor = sf::Color::Red;

    if (m_HoldTower)
    {
        sf::RectangleShape Rect(sf::Vector2f(64, 64));
        Rect.setPosition(sf::Vector2f(CellX * 64, CellY * 64));
        Rect.setOutlineThickness(3.f);
        Rect.setOutlineColor(GridColor);
        Rect.setFillColor(sf::Color::Transparent);
        Renderer.draw(Rect);
    }

    // RENDER INTERFEJSU
    DisplayTexture("Overlay.png", vec2i(0, 0), vec2i(SCREEN_WIDTH, SCREEN_HEIGHT));

    // ----------------------debug--------------------------------
    if (_gridDebug)
    {
        for(int _x = 0; _x < SCREEN_WIDTH; _x += 64)
        {
            for(int _y = 0; _y < SCREEN_HEIGHT; _y += 64)
            {
                int _CellX = (_x / 64);
                int _CellY = (_y / 64);

                eGridValue _grid_state = m_Grid[_CellY % GRID_ROWS][_CellX % GRID_COLS];

                if (_grid_state == eGridValue::FREE)
                GridColor = sf::Color::White;
                else if (_grid_state == eGridValue::BLOCKED)
                GridColor = sf::Color::Red;

                sf::RectangleShape Rect(sf::Vector2f(64, 64));
                Rect.setPosition(sf::Vector2f(_CellX * 64, _CellY * 64));
                Rect.setOutlineThickness(2.f);
                Rect.setOutlineColor(GridColor);
                Rect.setFillColor(sf::Color::Transparent);
                Renderer.draw(Rect);
            }
        }
    }

    // POZYCJA MYSZKI W PRAWYM DOLNYM ROGU
    m_Font->DrawText(Renderer, 1, 1860, 1050, ToString(MouseX).c_str());
    m_Font->DrawText(Renderer, 1, 1860, 1060, ToString(MouseY).c_str());

    // -----------------------------------------------------------

    // RENDER OBIEKTOW
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        m_AllGameObjects[i]->Render(Renderer);
    }

    // RYSOWANIE CZCIONKI
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

void InGameState::BuildTower(vec2i Position, const string& TowerName)
{
    auto pTower = make_shared<Tower>(Engine::GetSingleton()->GetMousePos(), TowerName);
    m_AllGameObjects.push_back(pTower);

    // sortowanie wiezy po pozycji y, aby wieze znajdujace sie "blizej" gracza byly widoczne na 1 planie
    sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), [](shared_ptr<GameObject> p1, shared_ptr<GameObject> p2) { return p1->GetPosition().y < p2->GetPosition().y;  });

    m_Grid[Position.y % GRID_ROWS][Position.x % GRID_COLS] = eGridValue::BLOCKED;
}

void InGameState::DestroyTextures()
{
    Engine::GetSingleton()->DestroyTextures();
}

void InGameState::DisplayTexture(const string& FileName, vec2i Position, optional<vec2i> Size)
{
    Engine::GetSingleton()->DisplayTexture(("../Data/" + FileName).c_str(), Position, Size);
}