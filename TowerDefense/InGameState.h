#pragma once
#include "GameState.h"
#include "AStar.h"
#include "Unit.h"
#include "ToolTip.h"
#include "Image.h"
#include "ParticleEmiter.h"

class Tower;

class InGameState : public GameState
{
public:
    InGameState();
    ~InGameState();
    void OnEnter()override;
    bool ReadGrid();
    void OnMouseButtonDown(int Button)override;
    void OnKeyDown(sf::Keyboard::Key KeyCode)override;
    void Render(sf::RenderWindow& Renderer)override;
    void Update(float DeltaTime)override;
    void BuildTower(vec2 Cell, const Definition* pDef);
    void CreateUnit(vec2 Position, const string& UnitName);
    shared_ptr<ParticleEmiter> CreateParticles(vec2 Position, int ParticleCount, float ParticleScale, float MaxLifeTime, shared_ptr<GameObject> Target=nullptr);
    void DisplayTexture(const string& FileName, vec2i Position, DisplayParameters Param = {});
    void DestroyTextures();
    void Shoot(shared_ptr<Tower> Source, shared_ptr<Unit> Target, Damage DamageValue);
    void EnableGroup(eUIGroup Group);
    void DisableGroup(eUIGroup Group);
    void InitializeCursor(eCursorType CursorType, string FilePath);
    void ChangeUnitPhase(const string& Name);
    void CreateGameObjects();

    template<typename T>
    vector<shared_ptr<T>> GetObjects();
    float GetMoneyAmount()const;

    void SetCursor(eCursorType Cursor);
    void SetMoneyAmount(float Value);

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
    float                                             m_Money = 1000.f;
    AStar                                             m_PathFinder;
    shared_ptr<Image>                                 m_UnitPhaseIcon;

    string m_CurrentUnitPhase;
    float                                             m_TimeToNextUnitPhase = 20.f;

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
