#pragma once
#include "GameObject.h"


class Button : public GameObject
{
public:
    Button(string TextureName, vec2i Position, vec2i Size, function<void()> Function);
    void Update(float DeltaTime) {};
    void Render(SDL_Renderer* pRenderer);
    void OnMouseButtonDown(int Button);
    bool IsCursorOnButton()const;

private:
    string m_TextureName;
    function<void()> m_Function;
};