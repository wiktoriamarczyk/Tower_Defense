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
    void OnMouseButtonDown(int Button);

private:
    SDL_Texture* m_pTexture = nullptr;
    SDL_Texture* m_pOverlayTexture = nullptr;
    SDL_Texture* m_pSomeTower = nullptr;
    vector<shared_ptr<GameObject>> m_AllGameObjects;
    // 0 - nie mozna polozyc obiektu, 1 - mozna polozyc obiekt, 2 - wybrany obiekt to "some_tower"
    char Table[17][30] = {};
    eTowerID m_TowerID = eTowerID::NONE;
    bool m_HoldTower = false;
    SDL_Cursor* m_Cursor;
};

