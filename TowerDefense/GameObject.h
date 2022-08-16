#pragma once
#include "Common.h"
#include "vec2.h"

class GameObject
{
public:
    virtual void Update(float DeltaTime) = 0;
    virtual void Render(sf::RenderWindow& Renderer) = 0;
    virtual bool OnMouseButtonDown(int Button) { return false; };
    virtual bool IsCursorOnButton()const { return false; };
    vec2 GetPosition()const;
    bool GetStatus()const;
    eGraphicLayer GetGraphicLayer()const;
    void SetPosition(vec2i Position);
    void SetStatus(bool Status);
    void SetGraphicLayer(eGraphicLayer Layer);
protected:
    string        m_Name;
    vec2          m_Position;
    vec2i         m_Size;
    bool          m_IsAlive = true;
    eGraphicLayer m_Layer = eGraphicLayer::FOREGROUND;
};