#pragma once
#include "GameObject.h"
#include "Common.h"
#include "Font.h"

class ToolTip : public GameObject
{
public:
    ToolTip(shared_ptr<Font> MyFont);
    void Update(float DeltaTime)override {};
    void Render(sf::RenderWindow& Renderer)override;
    void SetToolTip(vector<string> Data);

private:
    shared_ptr<Font> m_Font; 
    vector<string>   m_Data;
};