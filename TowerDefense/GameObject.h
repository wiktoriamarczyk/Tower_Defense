#pragma once
#include "Common.h"
#include "vec2.h"

class GameObject
{
public:
    virtual void Update(float DeltaTime) = 0;
    virtual void Render(SDL_Renderer* pRenderer) = 0;
    virtual bool OnMouseButtonDown(int Button) { return false; };
    vec2i GetPosition()const;
    bool GetStatus()const;
    void SetPosition(vec2i Position);
    void SetStatus(bool Status);
protected:
    vec2i m_Position;
    vec2i m_Size;
    bool m_IsAlive = true;
};