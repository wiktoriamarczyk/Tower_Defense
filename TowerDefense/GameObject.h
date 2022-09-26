#pragma once
#include "Common.h"
#include "vec2.h"

class ToolTip;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
    shared_ptr<GameObject> GetSelf() {return shared_from_this(); };
    virtual void Update(float DeltaTime) = 0;
    virtual void Render(sf::RenderWindow& Renderer) = 0;
    virtual bool OnMouseButtonDown(int Button) { return false; };
    virtual bool IsCursorOverObject()const { return false; };
    virtual bool FillToolTip(ToolTip& MyToolTip)const { return false; };

    vec2 GetPosition()const;
    vec2i GetSize()const;
    bool GetLifeStatus()const;
    eGraphicLayer GetGraphicLayer()const;
    bool IsActive()const;
    eUIGroup GetUIGroup()const;
    eCursorType GetCursor()const;

    void SetPosition(vec2 Position);
    void SetSize(vec2i Size);
    void SetLifeStatus(bool Status);
    void SetGraphicLayer(eGraphicLayer Layer);
    void SetActive(bool Status);
    void SetUIGroup(eUIGroup Group);
    void SetCursor(eCursorType Cursor);

protected:
    string        m_Name;
    TextureID     m_ID = TextureID::INVALID;
    vec2          m_Position;
    vec2i         m_Size;
    bool          m_Alive = true;
    eGraphicLayer m_Layer = eGraphicLayer::FOREGROUND;
    bool          m_Active = true;
    eUIGroup      m_UIGroup = eUIGroup::UNKNOWN;
    eCursorType   m_Cursor = eCursorType::DEFAULT;
};