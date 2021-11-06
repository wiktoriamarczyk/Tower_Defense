#include "InGameState.h"
#include "Engine.h"

InGameState::InGameState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer) : GameState(eStateID::INGAME)
{
    m_Font = MyFont;
    m_pRenderer = pRenderer;
}

InGameState::~InGameState()
{
    DestroyTextures();
}

void InGameState::DestroyTextures()
{
}

void InGameState::InitializeInGameStateTextures()
{
}

void InGameState::Update(float DeltaTime)
{
    if (SDL_IsKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        Mix_HaltChannel(-1);
        DestroyTextures();
        m_AllGameObjects.clear();
        m_NextStateID = eStateID::MAINMENU;
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