#include "GameObject.h"

vec2i GameObject::GetPosition()const
{
    return m_Position;
}

bool GameObject::GetStatus()const
{
    return m_IsAlive;
}

void GameObject::SetPosition(vec2i Position)
{
    m_Position = Position;
}

void GameObject::SetStatus(bool Status)
{
    m_IsAlive = Status;
}