#include "GameObject.h"

vec2i GameObject::GetObjectPosition()const
{
    return m_Position;
}

bool GameObject::GetObjectStatus()const
{
    return m_ObjectIsAlive;
}

void GameObject::SetObjectPosition(vec2i ObjectPos)
{
    m_Position = ObjectPos;
}

void GameObject::SetObjectStatus(bool ObjectStatus)
{
    m_ObjectIsAlive = ObjectStatus;
}