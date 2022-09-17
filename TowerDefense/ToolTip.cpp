#include "ToolTip.h"
#include "Engine.h"

void ToolTip::Render(sf::RenderWindow& Renderer)
{
    if (m_Lines.empty())
        return;

    int mouseX = Engine::GetSingleton()->GetMousePos().x;
    int mouseY = Engine::GetSingleton()->GetMousePos().y;

    float textMaxWidth = 0;
    float textMaxHeight = 0;

    for (size_t i = 0; i < m_Lines.size(); ++i)
    {
        auto Parameters = m_Lines[i].Parameters;
        Parameters.DisableDraw = true;

        vec2i textBounds = Engine::GetSingleton()->DrawText(m_Lines[i].Text, m_Lines[i].Size, vec2{0, 0}, m_Lines[i].Parameters);

        textMaxWidth = max(textMaxWidth, float(textBounds.x));
        textMaxHeight += textBounds.y;
    }

    // ustawienie rozmiau boxa do max szerokosci i wysokosci wyswietlanego tekstu
    vec2 boxSize(textMaxWidth + 40.f, textMaxHeight + 40.f);

    sf::RectangleShape box(boxSize);
    box.setFillColor(sf::Color::Black * sf::Color(255,255,255,128));
    box.setOutlineColor(sf::Color::Black * sf::Color(255,255,255,192));
    box.setOutlineThickness(2.f);

    // shift - przesuniecie w przypadku, gdy box mialby wyjsc poza ekran
    vec2 shift(0,0);

    if (mouseX + boxSize.x >= SCREEN_WIDTH)
    {
        shift.x = boxSize.x;
    }

    if (mouseY + boxSize.y >= SCREEN_HEIGHT)
    {
        shift.y = boxSize.y;
    }

    box.setPosition(vec2(mouseX - shift.x, mouseY - shift.y));

    // przesuniecie kazdej kolejnej linii w osi Y (kazda linia rysuje sie po sobie)
    float lineShift = 20.f;

    Renderer.draw(box);

    for (size_t i = 0; i < m_Lines.size(); ++i)
    {
       lineShift += Engine::GetSingleton()->DrawText(m_Lines[i].Text, m_Lines[i].Size, vec2{float(mouseX + boxSize.x / 2), float(mouseY + lineShift)} - shift, m_Lines[i].Parameters).y;
    }
}

void ToolTip::ClearToolTip()
{
    m_Lines.clear();
}

void ToolTip::AddToolTipLine(string Text, int Size, DrawTextParameters Param)
{
    Param.HAlign = eTextHorizontalAlign::CENTER;
    m_Lines.push_back(TextLineData{Text, Size, Param});
}