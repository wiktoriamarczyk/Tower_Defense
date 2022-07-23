#pragma once
#include "GameObject.h"


class Button : public GameObject
{
public:
    Button(string TextureName, vec2i Position, vec2i Size, function<void()> Function);
    void Update(float DeltaTime)override {};
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    bool IsCursorOnButton()const override;

private:
    string m_TextureName;
    function<void()> m_Function;
};