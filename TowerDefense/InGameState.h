#pragma once
#include "GameState.h"
#include "AStar.h"
#include "Unit.h"
#include "ToolTip.h"

class InGameState : public GameState
{
public:
    InGameState(shared_ptr<Font> MyFont);
    ~InGameState();
    void OnEnter()override;
    void OnMouseButtonDown(int Button)override;
    void OnKeyDown(sf::Keyboard::Key KeyCode)override;
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    void DisplayTexture(const string& FileName, vec2i Position, DisplayParameters Param = {});
    void DestroyTextures();
    void BuildTower(vec2 Cell, const Definition* pDef);
    void CreateUnit(vec2 Position, const string& UnitName);
    void Shoot(vec2 StartingPosition, shared_ptr<Unit> Target);
    bool ReadGrid();
    void CreateGameObjects();

    template<typename T>
    vector<shared_ptr<T>> GetObjects();

private:
    sf::Cursor                     m_CursorHand;
    sf::Cursor                     m_CursorArrow;

    vector<shared_ptr<GameObject>> m_AllGameObjects;
    shared_ptr<ToolTip>            m_ToolTip; 
    eGridValue                     m_Grid[GRID_ROWS][GRID_COLS] = {};
    //eTowerID                     m_PickedTowerID = eTowerID::NONE;
    bool                           m_HoldTower = false;
    bool                           m_MoveTower = false;
    const Definition*              m_pTowerDef = nullptr;
    int                            m_Money = 1000;
    AStar                          m_PathFinder;

    float                          m_SpawningTimer = 0;
    float                          m_UnitPhaseTimer = 40.f;
    string                         m_PhaseUnitName;

    // debug
    bool _gridDebug = false;
};

template<typename T>
vector<shared_ptr<T>> InGameState::GetObjects()
{
    vector<shared_ptr<T>> specificObjects;

    for (size_t i = 0; i < m_AllGameObjects.size(); ++i)
    {
        shared_ptr<T> object = dynamic_pointer_cast<T>(m_AllGameObjects[i]);

        if (object)
            specificObjects.push_back(object);
    }

    return specificObjects;
}
