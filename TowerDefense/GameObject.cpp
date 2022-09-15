#include "GameObject.h"
#include "Engine.h"

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
    return m_Alive;
}

eGraphicLayer GameObject::GetGraphicLayer() const
{
    return m_Layer;
}

bool GameObject::IsActive()const
{
    return m_Active;
}

eUIGroup GameObject::GetUIGroup()const
{
    return m_UIGroup;
}

eCursorType GameObject::GetCursor()const
{
    return m_Cursor;
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
    m_Alive = Status;
}

void GameObject::SetGraphicLayer(eGraphicLayer Layer)
{
    m_Layer = Layer;
}

void GameObject::SetActive(bool Status)
{
    m_Active = Status;
}

 void GameObject::SetUIGroup(eUIGroup Group)
 {
    m_UIGroup = Group;
 }

void GameObject::SetCursor(eCursorType Cursor)
{
    m_Cursor = Cursor;
}