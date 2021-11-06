#pragma once
#include "Common.h"
#include "vec2.h"

class GameObject
{
public:
    virtual void Update(float DeltaTime) = 0;
    virtual void Render(SDL_Renderer* pRenderer) = 0;
    vec2 GetObjectPosition();
    bool GetObjectStatus();
    void SetObjectPosition(vec2 ObjectPos);
    void SetObjectStatus(bool ObjectStatus);
protected:
    vec2 m_StartingPointPosition;
    vec2 m_ObjectSize;
    bool m_ObjectIsAlive = true;
};