#pragma once
#include "GameObject.h"

class Tower : public GameObject
{
public:
    Tower(vec2 Position, const string& TowerName);
    void Update(float DeltaTime);
    void Render(SDL_Renderer* pRenderer);
private:
    string m_Name;
    vec2i  m_TextureSize;
};

