#pragma once
#include "Common.h"
#include "GameObject.h"
#include "Font.h"

class Engine;

class GameState
{
public:
    GameState(eStateID StateID);
    virtual void Update(float DeltaTime) = 0;
    virtual void Render() = 0;
    virtual void OnEnter();
    virtual void OnKeyDown(SDL_Scancode KeyCode) {};
    virtual void OnMouseButtonDown(int Button) {};
    eStateID GetStateID()const;
    eStateID GetNextStateID()const;


protected:
    eStateID m_NextStateID = eStateID::UNKNOWN;
    shared_ptr<Font> m_Font;
    SDL_Renderer* m_pRenderer = nullptr;
    static bool m_GameOver;

private:
    const eStateID m_StateID = eStateID::UNKNOWN;
};

