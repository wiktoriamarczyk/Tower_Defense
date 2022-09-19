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

    template<typename T>
    vector<shared_ptr<T>> GetObjects();

private:
    int                            m_Option = 0;
    vector<shared_ptr<GameObject>> m_AllMenuObjects;
    bool                           m_PlayMusicAgain = false;
};

template<typename T>
vector<shared_ptr<T>> MainMenuState::GetObjects()
{
    vector<shared_ptr<T>> specificObjects;

    for (size_t i = 0; i < m_AllMenuObjects.size(); ++i)
    {
        shared_ptr<T> object = dynamic_pointer_cast<T>(m_AllMenuObjects[i]);

        if (object)
            specificObjects.push_back(object);
    }

    return specificObjects;
}