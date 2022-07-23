#include "Texture.h"

Texture::Texture(sf::RenderWindow* pRenderer)
{
    m_pRenderer = pRenderer;
}

Texture::~Texture() {}

bool Texture::Load(const string& FileName)
{
    m_FileName = FileName;

    if (!m_Texture.loadFromFile("../Data/" + FileName))
    {
        printf("Texture could not be loaded!\n");
        return false;
    }

    m_Size = m_Texture.getSize();

    return true;
}

void Texture::Display(vec2i Position, optional<vec2i> Size)const
{
    //int value = 0;
    //if( w.has_value() )
    //    value = w.value();
    
    //if (w)
    // value = *w;

    //vec2i tmp = Size.value_or(m_Size);

    //SDL_Rect Rect = { Position.x, Position.y, tmp.x, tmp.y };
    //SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, &Rect);

    sf::Sprite sprite;
    sprite.setTexture(m_Texture);
    sprite.setPosition(vec2(Position));

    m_pRenderer->draw(sprite);
}

void Texture::FreeResources()
{
    Texture::~Texture();
}

vec2i Texture::GetSize()
{
    return m_Size;
}

string Texture::GetName()
{
    return m_FileName;
}