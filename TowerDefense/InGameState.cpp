#include "InGameState.h"
#include "Engine.h"
#include "Tower.h"

InGameState::InGameState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer) : GameState(eStateID::INGAME)
{
    if (!ReadGrid())
    {
        std::cout << "File could not be loaded!" << std::endl;
        return;
    }
    m_Font = MyFont;
    m_pRenderer = pRenderer;
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

        for (int i = 0; i < 17; ++i)
        {
            for (int j = 0; j < 31;)
            {
                stream >> tmp_char;

                if (tmp_char != ',' && tmp_char != ';')
                {
                    Grid[i][j] = tmp_char;
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
    int CellX = (x / 64);
    int CellY = (y / 64);
    char Test = Grid[CellY % 17][CellX % 30];

    // wybieramy wieze do postawienia
    if (Test == '2' && m_TowerID == eTowerID::NONE)
    {
        m_TowerID = eTowerID::SOME_TOWER;
        m_HoldTower = true;
    }
    // jesli klikamy jeszcze raz na wybor wiezy, owczesnie ja trzymajac, oddajemy ja spowrotem do schowka
    else if (Test == '2' && m_TowerID == eTowerID::SOME_TOWER)
    {
        m_TowerID = eTowerID::NONE;
        m_HoldTower = false;
    }
    // jesli trzymamy wieze i jestesmy na poprawnym polu, to po kliknieciu LPM stawiamy wieze
    if (m_TowerID == eTowerID::SOME_TOWER)
    {
        if (Test == '1')
        {
            auto pTower = make_shared<Tower>(Engine::GetSingleton()->GetMousePos(), m_pSomeTower);
            m_AllGameObjects.push_back(pTower);
            Grid[CellY % 17][CellX % 30] = '0';
            Grid[CellY % 17][CellX % 30 + 1] = '0';  // nie mozna stawiac budynku od razu po prawej
            Grid[CellY % 17][CellX % 30 - 1] = '0';  // nie mozna stawiac budynku od razu po lewej
            m_TowerID = eTowerID::NONE;
            m_HoldTower = false;
        }
    }

}

InGameState::~InGameState()
{
    DestroyTextures();
}

void InGameState::DestroyTextures()
{
    SDL_DestroyTexture(m_pTexture);
    SDL_DestroyTexture(m_pOverlayTexture);
    SDL_DestroyTexture(m_pSomeTower);
    m_pTexture = nullptr;
    m_pOverlayTexture = nullptr;
    m_pSomeTower = nullptr;
}

void InGameState::InitializeInGameStateTextures()
{
    SDL_Surface* m_pImage = IMG_Load("../Data/Background.png");
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pImage);
    SDL_FreeSurface(m_pImage);

    m_pImage = IMG_Load("../Data/Overlay.png");
    m_pOverlayTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pImage);
    SDL_FreeSurface(m_pImage);

    m_pImage = IMG_Load("../Data/some_tower.png");
    m_pSomeTower = SDL_CreateTextureFromSurface(m_pRenderer, m_pImage);
    SDL_FreeSurface(m_pImage);
}

void InGameState::Update(float DeltaTime)
{
    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;
    int CellX = (x / 64);
    int CellY = (y / 64);
    char Test = Grid[CellY % 17][CellX % 30];

    if (SDL_IsKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        Mix_HaltChannel(-1);
        DestroyTextures();
        m_AllGameObjects.clear();
       // m_NextStateID = eStateID::MAINMENU;
        Engine::GetSingleton()->ExitGame();
    }

    if (Test == '2')
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
    
        if (m_AllGameObjects[i]->GetObjectStatus() == false)
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
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);


    SDL_Rect Map = { 0, 0, 1668, SCREEN_HEIGHT };
    //SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(pRenderer, &dstrect);
    SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, &Map);


    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;

    // numer w rzedzie i w kolumnie 
    int CellX = (x / 64);
    int CellY = (y / 64);

    char Test = Grid[CellY % 17][CellX % 30];

    if (Test == '1')
        SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
    else if (Test == '0')
        SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 255);
 /*   else if (Test == '2')
        SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, 255);*/


    if (m_HoldTower)
    {
        SDL_Rect rect = { CellX * 64, CellY * 64, 64, 64 };
        SDL_RenderDrawRect(m_pRenderer, &rect);
    }

    SDL_Rect Overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(m_pRenderer, m_pOverlayTexture, NULL, &Overlay);


    vec2 NormalPos(1650, 280);
    vec2 MovedPos(1640, 270);

    SDL_Rect Some_tower = { (int)NormalPos.x, (int)NormalPos.y, 97, 95 };

    if (m_MoveTower)
    {
        Some_tower = { (int)MovedPos.x, (int)MovedPos.y, 97, 95 };
    }

    SDL_RenderCopy(m_pRenderer, m_pSomeTower, NULL, &Some_tower);


    // render wszystkich obiektow
    for (int i = 0; i < m_AllGameObjects.size(); ++i)
    {
        m_AllGameObjects[i]->Render(m_pRenderer);
    }

    SDL_RenderPresent(m_pRenderer);
}

void InGameState::CreateObject()
{
}

void InGameState::OnEnter()
{
    m_GameOver = false;
    GameState::OnEnter();
    // inicjalizacja zasobow
    InitializeInGameStateTextures();
    CreateObject();
}