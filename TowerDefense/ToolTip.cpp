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

    // rozmiar jednej literki w pikselach
    const float letterPxSize = 8;
    // wielkosc odstepu miedzy kolejnymi wierszami
    const float lineSpacing = 2 * letterPxSize;
    // szerokosc tekstu, czyli najdluzszego wiersza (posiadajacego najwiecej liter)
    float textMaxWidth = 0;

    for (size_t i = 0; i < m_Data.size(); ++i)
    {
        float textWidth = m_Data[i].size() * letterPxSize; 
        textMaxWidth = std::max(textMaxWidth, textWidth);
    }
    // wysokosc tekstu - ilosc linii do wyswietlenia wraz z odstepami miedzy nimi
    float textMaxHeight = m_Data.size() * lineSpacing;
    // ustawienie rozmiau boxa do max szerokosci i wysokosci wyswietlanego tekstu
    vec2 boxSize(textMaxWidth + 40.f, textMaxHeight + 40.f);

    sf::RectangleShape box(boxSize);
    box.setFillColor(sf::Color::Black * sf::Color(255,255,255,128));
    box.setOutlineColor(sf::Color::Black * sf::Color(255,255,255,192));
    box.setOutlineThickness(2.f);

    vec2 shift(0,0);

    box.setPosition(vec2(mouseX, mouseY));

    if (mouseX + boxSize.x >= SCREEN_WIDTH)
    {
        shift.x = boxSize.x;
    }

    if (mouseY + boxSize.y >= SCREEN_HEIGHT)
    {
        shift.y = boxSize.y;
    }

    box.setPosition(vec2(mouseX - shift.x, mouseY - shift.y));
    Renderer.draw(box);

    for (size_t i = 0; i < m_Data.size(); ++i)
    {
        float textWidth = m_Data[i].size() * letterPxSize;
        //                                wysrodkowanie w boxie  wysrodk. tekstu                   
        m_Font->DrawText(Renderer, 1, (mouseX + boxSize.x / 2 - textWidth / 2 - shift.x), (mouseY + i * lineSpacing - shift.y + 25), m_Data[i].c_str());
    }
}

void ToolTip::InitializeToolTipText(vector<string> Data)
{
    m_Data = Data;
}