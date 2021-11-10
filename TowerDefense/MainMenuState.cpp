#include "MainMenuState.h"
#include "Engine.h"

MainMenuState::MainMenuState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer) : GameState(eStateID::MAINMENU)
{
    m_Font = MyFont;
    m_pRenderer = pRenderer;
}

MainMenuState::~MainMenuState()
{
    DestroyTextures();
}

void MainMenuState::DestroyTextures()
{
}

void MainMenuState::InitializeMainMenuStateTextures()
{
}

void MainMenuState::OnEnter()
{
    GameState::OnEnter();
    InitializeMainMenuStateTextures();
}

void MainMenuState::Update(float DeltaTime) 
{
}

/*

        Kocham! 

              ******       ******
            **********   **********
          ************* *************
         *****************************
         *****************************
         *****************************
          ***************************
            ***********************
              *******************
                ***************
                  ***********
                    *******
                      ***
                       *
*/

void MainMenuState::Render()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);

    SDL_RenderPresent(m_pRenderer);
}

void MainMenuState::OnKeyDown(SDL_Scancode KeyCode)
{
    // jesli gracz wciska esc, wywolaj zamkniecie programu
    if (KeyCode == SDL_SCANCODE_ESCAPE)
    {
        Engine::GetSingleton()->ExitGame();
    }

    // sprawdzenie czy gracz naciska klawisz w dol, by przejsc do opcji ponizej 
    if (KeyCode == SDL_SCANCODE_DOWN && m_Option < 1)
    {
        m_Option++;
    }

    // sprawdzenie czy gracz naciska klawisz w gore, by przejsc do opcji powyzej
    else if (KeyCode == SDL_SCANCODE_UP && m_Option > 0)
    {
        m_Option--;
    }

    // po nacisieciu enter dostajemy sie do okna gry lub wychodzimy z gry
    else if (KeyCode == SDL_SCANCODE_RETURN)
    {
        if (m_Option == 0)
        {
            m_PlayMusicAgain = true;
            Mix_HaltChannel(-1);
            DestroyTextures();
            m_NextStateID = eStateID::INGAME;
        }
        if (m_Option == 1)
        {
            Engine::GetSingleton()->ExitGame();
        }
    }
}
