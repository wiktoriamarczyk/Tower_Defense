#include "GameObject.h"

vec2 GameObject::GetObjectPosition()
{
    return m_StartingPointPosition;
}

bool GameObject::GetObjectStatus()
{
    return m_ObjectIsAlive;
}

void GameObject::SetObjectPosition(vec2 ObjectPos)
{
    m_StartingPointPosition = ObjectPos;
}

void GameObject::SetObjectStatus(bool ObjectStatus)
{
    m_ObjectIsAlive = ObjectStatus;
}