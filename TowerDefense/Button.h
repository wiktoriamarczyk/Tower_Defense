#pragma once
#include "GameObject.h"
#include "Definition.h"

class Button : public GameObject
{
public:
    Button(string TextureName, vec2i Position, vec2i Size, function<void()> Function);
    void Update(float DeltaTime)override {};
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    bool IsCursorOverObject()const override;
    vector<string> GetToolTip()const override;
    void SetToolTipText(vector<string> ToolTip);

private:
    string           m_TextureName;
    vector<string>   m_ToolTip;
    function<void()> m_Function;
};

class TowerButton : public Button
{
public:
    using Button::Button;

    vector<string> GetToolTip()const override;
    void SetDefinition(const Definition* pDef);
    
private:
    const Definition* m_pDef = nullptr;
};
