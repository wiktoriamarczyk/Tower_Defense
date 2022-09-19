#include "MainMenuState.h"
#include "Engine.h"
#include "Image.h"
#include "Button.h"

MainMenuState::MainMenuState() : GameState(eStateID::MAINMENU) 
{
    // tworzenie obiektow
    shared_ptr<Image> backgroundImage = make_shared<Image>("Images/MenuBackground.jpg", vec2(0, 0), vec2(0, 0));
    backgroundImage->SetGraphicLayer(eGraphicLayer::BACKGROUND);
    m_AllMenuObjects.push_back(backgroundImage);


    auto func = [this]()
    {
        m_NextStateID = eStateID::INGAME;
    };

    auto func2 = [this]()
    {
        m_NextStateID = eStateID::INGAME;
    };

    auto func3 = [this]()
    {
        
    };

    auto func4 = [this]()
    {
        
            
    };

    auto func5 = [this]()
    {
        Engine::GetSingleton()->ExitGame();
    };

    shared_ptr<Button> newGameButton = make_shared<Button>("/Buttons/NewGameButton.png", vec2(1500, 200), vec2(0.5, 0.5), func, true);
    m_AllMenuObjects.push_back(newGameButton);

    shared_ptr<Button> loadGameButton = make_shared<Button>("/Buttons/LoadGameButton.png", vec2(1500, 350), vec2(0.5, 0.5), func2, true);
    m_AllMenuObjects.push_back(loadGameButton);

    shared_ptr<Button> bestScoresButton = make_shared<Button>("/Buttons/BestScoresButton.png", vec2(1500, 500), vec2(0.5, 0.5), func3, true);
    m_AllMenuObjects.push_back(bestScoresButton);

    shared_ptr<Button> authorsButton = make_shared<Button>("/Buttons/AuthorsButton.png", vec2(1500, 650), vec2(0.5, 0.5), func4, true);
    m_AllMenuObjects.push_back(authorsButton);

    shared_ptr<Button> exitButton = make_shared<Button>("/Buttons/ExitGameButton.png", vec2(1500, 800), vec2(0.5, 0.5), func5, true);
    m_AllMenuObjects.push_back(exitButton);
}

MainMenuState::~MainMenuState() {}

void MainMenuState::OnEnter()
{
    GameState::OnEnter();
}

void MainMenuState::OnMouseButtonDown(int Button)
{
    for (int i = 0; i < m_AllMenuObjects.size(); ++i)
    {
        if (m_AllMenuObjects[i]->IsActive() && m_AllMenuObjects[i]->OnMouseButtonDown(Button))
            return;
    }
}

void MainMenuState::OnKeyDown(sf::Keyboard::Key KeyCode)
{}

void MainMenuState::Update(float DeltaTime) 
{
    // update obiektow
     for (size_t i = 0; i < m_AllMenuObjects.size();)
    {
        if (m_AllMenuObjects[i]->IsActive())
            m_AllMenuObjects[i]->Update(DeltaTime);

        if (m_AllMenuObjects[i]->GetLifeStatus() == false)
            m_AllMenuObjects.erase(m_AllMenuObjects.begin() + i);

        else
            ++i;
    }
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

    for (int i = 0; i < m_AllMenuObjects.size(); ++i)
    {
        if (m_AllMenuObjects[i]->IsActive())
            m_AllMenuObjects[i]->Render(Renderer);
    }

    Renderer.display();
}