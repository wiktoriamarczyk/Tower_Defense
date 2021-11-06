#pragma once
#include "GameState.h"

class InGameState : public GameState
{
public:
    InGameState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer);
    ~InGameState();
    void InitializeInGameStateTextures();
    void Update(float DeltaTime)override;
    void Render()override;
    void OnEnter()override;
    void CreateObject();
    void DestroyTextures();

private:
    vector<shared_ptr<GameObject>> m_AllGameObjects;
};

