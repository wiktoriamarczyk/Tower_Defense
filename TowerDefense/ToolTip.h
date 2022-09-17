#pragma once
#include "GameObject.h"
#include "Common.h"

class ToolTip : public GameObject
{
public:
    ToolTip()=default;
    void Update(float DeltaTime)override {};
    void Render(sf::RenderWindow& Renderer)override;
    void ClearToolTip();
    void AddToolTipLine(string Text, int Size = 10, DrawTextParameters Param = {});

private:
    vector<TextLineData> m_Lines;
};