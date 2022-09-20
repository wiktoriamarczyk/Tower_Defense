#pragma once
#include "GameState.h"
#include "GameObject.h"

class MainMenuState : public GameState
{
public:
    MainMenuState();
    ~MainMenuState();
    void OnEnter()override;
    void OnMouseButtonDown(int Button)override;
    void OnKeyDown(sf::Keyboard::Key KeyCode)override;
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;

private:
    int                            m_Option = 0;
    vector<shared_ptr<GameObject>> m_AllMenuObjects;
    bool                           m_PlayMusicAgain = false;
};