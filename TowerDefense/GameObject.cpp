#include "GameObject.h"

vec2 GameObject::GetPosition()const
{
    return m_Position;
}

vec2i GameObject::GetSize()const
{
    return m_Size;
}

bool GameObject::GetLifeStatus()const
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

void GameObject::SetSize(vec2i Size)
{
    m_Size = Size;
}

void GameObject::SetLifeStatus(bool Status)
{
    m_IsAlive = Status;
}

void GameObject::SetGraphicLayer(eGraphicLayer Layer)
{
    m_Layer = Layer;
}