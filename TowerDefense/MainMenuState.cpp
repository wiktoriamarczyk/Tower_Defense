#include "MainMenuState.h"
#include "Engine.h"

MainMenuState::MainMenuState(shared_ptr<Font> MyFont) : GameState(eStateID::MAINMENU)
{
    m_Font = MyFont;
}

MainMenuState::~MainMenuState()
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

void MainMenuState::Render(sf::RenderWindow& Renderer)
{
    Renderer.clear(sf::Color::Black);
    Renderer.display();

}

void MainMenuState::OnKeyDown(sf::Keyboard::Key KeyCode)
{
    // jesli gracz wciska esc, wywolaj zamkniecie programu
    if (KeyCode == sf::Keyboard::Escape)
    {
        Engine::GetSingleton()->ExitGame();
    }

    // sprawdzenie czy gracz naciska klawisz w dol, by przejsc do opcji ponizej 
    if (KeyCode == sf::Keyboard::Down && m_Option < 1)
    {
        m_Option++;
    }

    // sprawdzenie czy gracz naciska klawisz w gore, by przejsc do opcji powyzej
    else if (KeyCode == sf::Keyboard::Up && m_Option > 0)
    {
        m_Option--;
    }

    // po nacisieciu enter dostajemy sie do okna gry lub wychodzimy z gry
    else if (KeyCode == sf::Keyboard::Enter)
    {
        if (m_Option == 0)
        {
            m_PlayMusicAgain = true;
            //! Mix_HaltChannel(-1);
            m_NextStateID = eStateID::INGAME;
        }
        if (m_Option == 1)
        {
            Engine::GetSingleton()->ExitGame();
        }
    }
}
