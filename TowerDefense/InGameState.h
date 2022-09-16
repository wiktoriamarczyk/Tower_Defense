#pragma once
#include "GameState.h"
#include "AStar.h"
#include "Unit.h"
#include "ToolTip.h"
#include "Image.h"

class Tower;

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
    void Shoot(shared_ptr<Tower> Source, shared_ptr<Unit> Target, Damage DamageValue);
    bool ReadGrid();
    void CreateGameObjects();
    void ChangeUnitPhase(const string& Name);
    void EnableGroup(eUIGroup Group);
    void DisableGroup(eUIGroup Group);
    void InitializeCursor(eCursorType CursorType, string FilePath);

    template<typename T>
    vector<shared_ptr<T>> GetObjects();

    void SetCursor(eCursorType Cursor);

private:
    eCursorType                                       m_CurrentCursor;
    vector<unique_ptr<pair<eCursorType, sf::Cursor>>> m_AllCursors;
    vector<shared_ptr<GameObject>>                    m_AllGameObjects;
    shared_ptr<ToolTip>                               m_ToolTip; 
    eGridValue                                        m_Grid[GRID_ROWS][GRID_COLS] = {};
    //eTowerID                                        m_PickedTowerID = eTowerID::NONE;
    bool                                              m_HoldTower = false;
    bool                                              m_MoveTower = false;
    const Definition*                                 m_pTowerDef = nullptr;
    int                                               m_Money = 1000;
    AStar                                             m_PathFinder;

    float                          m_SpawningTimer = 0;
    float                          m_UnitPhaseTimer = 40.f;
    float                          m_TimeToNextUnitPhase = 20.f;
    shared_ptr<Image>              m_UnitPhaseIcon;

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
