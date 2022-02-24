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
    void DisplayTexture(const string& FileName, vec2i Position, optional<vec2i> Size = nullopt);
    void DestroyTextures();
    void BuildTower(vec2i Position, const string& TowerName);
    bool ReadGrid();

private:
    SDL_Cursor*                    m_CursorHand = nullptr;
    SDL_Cursor*                    m_CursorArrow = nullptr;

    vector<shared_ptr<GameObject>> m_AllGameObjects;

    eGridValue                     m_Grid[GRID_ROWS][GRID_COLS] = {};
    //eTowerID                       m_PickedTowerID = eTowerID::NONE;
    bool                           m_HoldTower = false;
    bool                           m_MoveTower = false;
    string                         m_TowerName;
    int                            m_TowerCost = 0;
    int                            m_Money = 1000;
};

