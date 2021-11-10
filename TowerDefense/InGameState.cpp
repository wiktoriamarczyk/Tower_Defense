#include "InGameState.h"
#include "Engine.h"


InGameState::InGameState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer) : GameState(eStateID::INGAME)
{
    if (!ReadMap())
    {
        std::cout << "File could not be loaded!" << std::endl;
        return;
    }
    m_Font = MyFont;
    m_pRenderer = pRenderer;
}

bool InGameState::ReadMap()
{
    fstream stream("../Data/Map.txt");
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

        for (int i = 0; i < 16; ++i)
        {
            for (int j = 0; j < 27;)
            {
                stream >> tmp_char;

                if (tmp_char != ',' && tmp_char != ';')
                {
                    Table[i][j] = tmp_char;
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


InGameState::~InGameState()
{
    DestroyTextures();
}

void InGameState::DestroyTextures()
{
    SDL_DestroyTexture(m_pTexture);
    SDL_DestroyTexture(m_pOverlayTexture);
    m_pTexture = nullptr;
    m_pOverlayTexture = nullptr;
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
    if (SDL_IsKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        Mix_HaltChannel(-1);
        DestroyTextures();
        m_AllGameObjects.clear();
       // m_NextStateID = eStateID::MAINMENU;
        Engine::GetSingleton()->ExitGame();
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

    int CellX = (x / 64);
    int CellY = (y / 64);

    char Test = Table[CellY % 16][CellX % 26];

    if (Test == '1')
        SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 255);

    SDL_Rect rect = { CellX * 64, CellY * 64, 64, 64 };
    SDL_RenderDrawRect(m_pRenderer, &rect);

    SDL_Rect Overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(m_pRenderer, m_pOverlayTexture, NULL, &Overlay);

    SDL_Rect Some_tower = { 1640, 280, 97, 95 };
    SDL_RenderCopy(m_pRenderer, m_pSomeTower, NULL, &Some_tower);

    SDL_RenderPresent(m_pRenderer);



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

// SCREEN_WIDTH / INVADER_WIDTH - 3: 12 invaderow po 50 pikseli (lacznie zajmuja 600 pikseli)
// ROW * (SCREEN_WIDTH / 100): z odstepami po 8 pikseli miedzy kazdym (lacznie 100 pikseli)
// z 50 pikselowymi przerwami na poczatku i koncu ekranu (100 pikseli)
// 800 pikseli szerokosci ekranu

void InGameState::OnEnter()
{
    m_GameOver = false;
    GameState::OnEnter();
    // inicjalizacja zasobow
    InitializeInGameStateTextures();
    CreateObject();
}