#include "Image.h"
#include "Engine.h"

Image::Image(const string& Name, vec2 Position, vec2 Pivot)
{
    m_Name = Name;
    SetPosition(Position);
    m_Pivot = Pivot;
}

void Image::Update(float DeltaTime)
{

}

void Image::Render(sf::RenderWindow& Renderer)
{
    Engine::GetSingleton()->DisplayTexture((m_Name + ".png").c_str(), GetPosition(), DisplayParameters{.Pivot = m_Pivot});
}
