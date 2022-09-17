#pragma once
#include "GameState.h"

class MainMenuState : public GameState
{
public:
    MainMenuState();
    ~MainMenuState();
    void InitializeMainMenuStateTextures();
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    void OnKeyDown(sf::Keyboard::Key KeyCode)override;
    void OnEnter()override;

private:
    int  m_Option = 0;
    bool m_PlayMusicAgain = true;
};

