#pragma once
#include "Common.h"
#include "GameObject.h"

class Engine;

class GameState
{
public:
    GameState(eStateID StateID);
    virtual void Update(float DeltaTime) = 0;
    virtual void Render(sf::RenderWindow& Renderer) = 0;
    virtual void OnEnter();
    virtual void OnKeyDown(sf::Keyboard::Key KeyCode) {};
    virtual void OnMouseButtonDown(int Button) {};
    eStateID GetStateID()const;
    eStateID GetNextStateID()const;

protected:
    eStateID            m_NextStateID = eStateID::UNKNOWN;
    static bool         m_GameOver;

private:
    const eStateID      m_StateID = eStateID::UNKNOWN;
};

