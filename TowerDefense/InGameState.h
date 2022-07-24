#pragma once
#include "GameState.h"

class InGameState : public GameState
{
public:
    InGameState(shared_ptr<Font> MyFont, SDL_Renderer* pRenderer);
    ~InGameState();
    void OnEnter()override;
    void OnMouseButtonDown(int Button);
    void Update(float DeltaTime)override;
    void Render()override;
    void DisplayTexture(const string& FileName, int x, int y, optional<int> w = nullopt, optional<int> h = nullopt);
    void DestroyTextures();
    void BuildTower(int CellX, int CellY, const string& TowerName);
    bool ReadGrid();

private:
    SDL_Cursor*                    m_Cursor = nullptr;

    vector<shared_ptr<GameObject>> m_AllGameObjects;

    eGridValue                     m_Grid[GRID_ROWS][GRID_COLS] = {};
    eTowerID                       m_PickedTowerID = eTowerID::NONE;
    bool                           m_HoldTower = false;
    bool                           m_MoveTower = false;

    int                            m_Money = 1000;
};

