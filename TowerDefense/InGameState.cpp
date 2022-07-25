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


InGameState::~InGameState()
{
    //SDL_FreeCursor(m_CursorArrow);
    //SDL_FreeCursor(m_CursorHand);
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

void InGameState::Update(float DeltaTime)
{
    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;
    int CellX = (x / 64);
    int CellY = (y / 64);
    eGridValue grid_state = m_Grid[CellY % 17][CellX % 30];

    // PRZEŁĄCZANIE STANOW

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        //Mix_HaltChannel(-1);
        ///DestroyTextures();
        m_AllGameObjects.clear();
       // m_NextStateID = eStateID::MAINMENU;
        Engine::GetSingleton()->ExitGame();
    }

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
    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;
    int CellX = (x / 64);
    int CellY = (y / 64);
    eGridValue grid_state = m_Grid[CellY % GRID_ROWS][CellX % GRID_COLS];

    Renderer.clear(sf::Color::Black);

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

    DisplayTexture("Overlay.png", vec2i(0, 0), vec2i(SCREEN_WIDTH, SCREEN_HEIGHT));

    // RENDER OBIEKTOW
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        m_AllGameObjects[i]->Render(Renderer);
    }

    // CZCIONKI
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
    // inicjalizacja zasobow
    ///InitializeInGameStateTextures();
}

void InGameState::BuildTower(vec2i Position, const string& TowerName)
{
    auto pTower = make_shared<Tower>(Engine::GetSingleton()->GetMousePos(), TowerName);
    m_AllGameObjects.push_back(pTower);

    // posortuj wieze po pozycji y
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