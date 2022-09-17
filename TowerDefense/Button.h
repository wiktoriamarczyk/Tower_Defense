#pragma once
#include "GameObject.h"
#include "Definition.h"

class Button : public GameObject
{
public:
    Button(string TextureName, vec2 Position, vec2i Size, function<void()> Function, bool Move = false);
    void Update(float DeltaTime)override {};
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    bool IsCursorOverObject()const override;
    bool FillToolTip(ToolTip& MyToolTip)const override;
    void SetToolTipText(vector<TextLineData> ToolTip);

private:
    string                      m_TextureName;
    vector<TextLineData>   m_ToolTip;
    function<void()>            m_Function;
    bool                        m_Move = false;
};

class TowerButton : public Button
{
public:
    using Button::Button;

    bool FillToolTip(ToolTip& MyToolTip)const override;
    void SetDefinition(const Definition* pDef);

private:
    const Definition* m_pDef = nullptr;
};
