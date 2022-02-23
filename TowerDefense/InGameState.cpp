#include "InGameState.h"
#include "Engine.h"
#include "Tower.h"
#include "Button.h"

InGameState::InGameState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer) : GameState(eStateID::INGAME)
{
    if (!ReadGrid())
    {
        std::cout << "File could not be loaded!" << std::endl;
        return;
    }

    m_Font = MyFont;
    m_pRenderer = pRenderer;

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
    DestroyTextures();
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
    int CellX = (x / 64);
    int CellY = (y / 64);
    // 0 - nie mozna polozyc obiektu, 1 - mozna polozyc obiekt, 2 - wybrany obiekt to wieza
    eGridValue grid_state = m_Grid[CellY % GRID_ROWS][CellX % GRID_COLS];


    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        if (m_AllGameObjects[i]->OnMouseButtonDown(Button))
            return;
    }

    if (m_HoldTower)
    {
        if (grid_state == eGridValue::FREE)
        {
            BuildTower(CellX, CellY, m_TowerName);
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

    if (SDL_IsKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        Mix_HaltChannel(-1);
        ///DestroyTextures();
        m_AllGameObjects.clear();
       // m_NextStateID = eStateID::MAINMENU;
        Engine::GetSingleton()->ExitGame();
    }

    if (grid_state != eGridValue::FREE && grid_state != eGridValue::BLOCKED)
    {
        m_Cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        SDL_SetCursor(m_Cursor);
        m_MoveTower = true;
    }
    else
    {
        m_Cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        SDL_SetCursor(m_Cursor);
        m_MoveTower = false;
    }


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

void InGameState::Render()
{
    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;
    int CellX = (x / 64);
    int CellY = (y / 64);
    eGridValue grid_state = m_Grid[CellY % GRID_ROWS][CellX % GRID_COLS];

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);

    //SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(pRenderer, &dstrect);

    ///Engine::GetSingleton()->DisplayTexture("Background.png", 0, 0, 1668, SCREEN_HEIGHT);
    DisplayTexture("Background.png", 0, 0, 1668, SCREEN_HEIGHT);

    if (grid_state == eGridValue::FREE)
        SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
    else if (grid_state == eGridValue::BLOCKED)
        SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 255);


    if (m_HoldTower)
    {
        SDL_Rect rect = { CellX * 64, CellY * 64, 64, 64 };
        SDL_RenderDrawRect(m_pRenderer, &rect);
    }

    DisplayTexture("Overlay.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    // render ikon obiektow



    //vec2 NormalPos(1660, 290);
    //vec2 MovedPos(1650, 280);

    //if (grid_state == eGridValue::Tower1 && m_MoveTower)
    //{
    //    DisplayTexture("Tower1.png", (int)MovedPos.x, (int)MovedPos.y);
    //}
    //else 
    //    DisplayTexture("Tower1.png", (int)NormalPos.x, (int)NormalPos.y);


    //----------------------------------------------------------------------------

    /*if (grid_state == eGridValue::Tower2 && m_MoveTower)
    {
        DisplayTexture("Tower2.png", (int)MovedPos.x + 150, (int)MovedPos.y - 4);
    }
    else DisplayTexture("Tower2.png", (int)NormalPos.x + 160, (int)NormalPos.y - 4);*/


    // render wszystkich obiektow
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        m_AllGameObjects[i]->Render(m_pRenderer);
    }

    // CZCIONKI

    m_Font->DrawText(m_pRenderer, 1, 60, 1058, ToString(m_Money).c_str());

    if (m_Money < (int)eTowerPrice::Tower1)
    {
        m_Font->DrawText(m_pRenderer, 1, 1700, 388, ToString((int)eTowerPrice::Tower1).c_str(), 255, 0, 0);
    }
    else m_Font->DrawText(m_pRenderer, 1, 1700, 388, ToString((int)eTowerPrice::Tower1).c_str());


    if (m_Money < (int)eTowerPrice::Tower2)
    {
        m_Font->DrawText(m_pRenderer, 1, 1835, 388, ToString((int)eTowerPrice::Tower2).c_str(), 255, 0, 0);
    }
    else m_Font->DrawText(m_pRenderer, 1, 1835, 388, ToString((int)eTowerPrice::Tower2).c_str());

    SDL_RenderPresent(m_pRenderer);
}

void InGameState::OnEnter()
{
    m_GameOver = false;
    GameState::OnEnter();
    // inicjalizacja zasobow
    ///InitializeInGameStateTextures();
}

void InGameState::BuildTower(int CellX, int CellY, const string& TowerName)
{
    auto pTower = make_shared<Tower>(Engine::GetSingleton()->GetMousePos(), TowerName);
    m_AllGameObjects.push_back(pTower);

    // posortuj wieze po pozycji y
    sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), [](shared_ptr<GameObject> p1, shared_ptr<GameObject> p2) { return p1->GetPosition().y < p2->GetPosition().y;  });

    m_Grid[CellY % GRID_ROWS][CellX % GRID_COLS] = eGridValue::BLOCKED;

    //if (CellX < (GRID_COLS - 1))
    //{
    //    m_Grid[CellY % GRID_ROWS][CellX % GRID_COLS + 1] = eGridValue::BLOCKED;  // nie mozna stawiac budynku od razu po prawej
    //}

    //if (CellX > 0)
    //{
    //    m_Grid[CellY % GRID_ROWS][CellX % GRID_COLS - 1] = eGridValue::BLOCKED;  // nie mozna stawiac budynku od razu po lewej
    //}
}

void InGameState::DestroyTextures()
{
    Engine::GetSingleton()->DestroyTextures();
}

void InGameState::DisplayTexture(const string& FileName, int x, int y, optional<int> w, optional<int> h)
{
    Engine::GetSingleton()->DisplayTexture(("../Data/" + FileName).c_str(), x, y, w, h);
}