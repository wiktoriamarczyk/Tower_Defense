#include "MainMenuState.h"
#include "Engine.h"

MainMenuState::MainMenuState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer) : GameState(eStateID::MAINMENU)
{
    if (ReadMap() == false)
    {
        std::cout << "File could not be loaded!" << std::endl;
        return;
    }
    m_Font = MyFont;
    m_pRenderer = pRenderer;
}

bool MainMenuState::ReadMap()
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


MainMenuState::~MainMenuState()
{
    DestroyTextures();
}

void MainMenuState::DestroyTextures()
{
    SDL_DestroyTexture(m_pTexture);
    SDL_DestroyTexture(m_pOverlayTexture);
    m_pTexture = nullptr;
    m_pOverlayTexture = nullptr;
}

void MainMenuState::InitializeMainMenuStateTextures()
{
    SDL_Surface* m_pImage = IMG_Load("../Data/Background.png");
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pImage);
    SDL_FreeSurface(m_pImage);

    m_pImage = IMG_Load("../Data/Overlay.png");
    m_pOverlayTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pImage);
    SDL_FreeSurface(m_pImage);
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
   
    SDL_Rect dstrect = { 0, 0, 1668, SCREEN_HEIGHT };
    //SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(pRenderer, &dstrect);
    SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, &dstrect);


    int x = Engine::GetSingleton()->GetMousePos().x;
    int y = Engine::GetSingleton()->GetMousePos().y;

    int CellX = (x/64);
    int CellY = (y/64);

    char Test = Table[CellY%16][CellX%26];
    
    if( Test == '1' )
        SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 255);

    SDL_Rect rect = {CellX*64, CellY*64, 64, 64};
    SDL_RenderDrawRect(m_pRenderer, &rect);
    
    SDL_Rect Overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(m_pRenderer, m_pOverlayTexture, NULL, &Overlay);

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
