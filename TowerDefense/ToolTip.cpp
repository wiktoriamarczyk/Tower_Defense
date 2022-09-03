#include "ToolTip.h"
#include "Engine.h"

ToolTip::ToolTip(shared_ptr<Font> MyFont)
{
    m_Font = MyFont;
}

void ToolTip::Render(sf::RenderWindow& Renderer)
{
    if (m_Data.empty())
        return;
    
    int mouseX = Engine::GetSingleton()->GetMousePos().x;
    int mouseY = Engine::GetSingleton()->GetMousePos().y;

    sf::RectangleShape box(vec2(200.f, 200.f));
    box.setFillColor(sf::Color::Black * sf::Color(255,255,255,128));
    box.setOutlineColor(sf::Color::Black * sf::Color(255,255,255,192));
    box.setPosition(vec2(mouseX, mouseY) - vec2(100.f, 100.f));

    Renderer.draw(box);

    for (size_t i = 0; i < m_Data.size(); ++i)
    {
        m_Font->DrawText(Renderer, 1, mouseX - m_Data[i].size() * 8 / 2, mouseY - 8 / 2 + i * 16, m_Data[i].c_str());
    }
}

void ToolTip::SetToolTip(vector<string> Data)
{
    m_Data = Data;
}