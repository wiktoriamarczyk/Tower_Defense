#include "GameState.h"

bool GameState::m_GameOver = false;

GameState::GameState(eStateID StateID) : m_StateID(StateID) {}


eStateID GameState::GetStateID()const
{
    return m_StateID;
}

eStateID GameState::GetNextStateID()const
{
    return m_NextStateID;
}

void GameState::OnEnter()
{
    m_NextStateID = eStateID::UNKNOWN;
}