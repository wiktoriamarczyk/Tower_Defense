#include "GameObject.h"

vec2 GameObject::GetPosition()const
{
    return m_Position;
}

bool GameObject::GetStatus()const
{
    return m_IsAlive;
}

eGraphicLayer GameObject::GetGraphicLayer() const
{
    return m_Layer;
}

void GameObject::SetPosition(vec2 Position)
{
    m_Position = Position;
}

void GameObject::SetStatus(bool Status)
{
    m_IsAlive = Status;
}

void GameObject::SetGraphicLayer(eGraphicLayer Layer)
{
    m_Layer = Layer;
}
