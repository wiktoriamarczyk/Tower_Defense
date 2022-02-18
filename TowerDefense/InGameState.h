#pragma once
#include "GameState.h"

class InGameState : public GameState
{
public:
    InGameState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer);
    ~InGameState();
    void OnEnter()override;
    void Update(float DeltaTime)override;
    void Render()override;
    void InitializeInGameStateTextures();
    void DestroyTextures();
    void CreateObject(int CellX, int CellY, SDL_Texture* pTextureName);
    bool ReadGrid();
    void OnMouseButtonDown(int Button);
    bool CanAfford(eTowerPrice TowerName);

private:
    SDL_Texture*                   m_pBackground = nullptr;
    SDL_Texture*                   m_pOverlayTexture = nullptr;
    SDL_Texture*                   m_pTower1 = nullptr;
    SDL_Texture*                   m_pTower2 = nullptr;

    SDL_Cursor*                    m_Cursor = nullptr;

    vector<shared_ptr<GameObject>> m_AllGameObjects;

    eGridValue                     m_Grid[GRID_ROWS][GRID_COLS] = {};
    eTowerID                       m_TowerID = eTowerID::NONE;
    bool                           m_HoldTower = false;
    bool                           m_MoveTower = false;

    int                            m_Money = 1000;
};

