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
    bool ReadMap();

private:
    SDL_Texture* m_pTexture = nullptr;
    SDL_Texture* m_pOverlayTexture = nullptr;
    SDL_Texture* m_pSomeTower = nullptr;
    vector<shared_ptr<GameObject>> m_AllGameObjects;
    char Table[16][26] = {};
};

