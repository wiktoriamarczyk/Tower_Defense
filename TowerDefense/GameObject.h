#pragma once
#include "Common.h"
#include "vec2.h"

class GameObject
{
public:
    virtual void Update(float DeltaTime) = 0;
    virtual void Render(SDL_Renderer* pRenderer) = 0;
    virtual void OnMouseButtonDown(int Button) {};
    vec2i GetObjectPosition()const;
    bool GetObjectStatus()const;
    void SetObjectPosition(vec2i ObjectPos);
    void SetObjectStatus(bool ObjectStatus);
protected:
    vec2i m_Position;
    vec2i m_ObjectSize;
    bool m_ObjectIsAlive = true;
};