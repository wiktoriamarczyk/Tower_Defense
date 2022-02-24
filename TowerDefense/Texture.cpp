#include "Texture.h"

Texture::Texture(SDL_Renderer* pRenderer)
{
    m_pRenderer = pRenderer;
}

Texture::~Texture()
{
    SDL_DestroyTexture(m_pTexture);
    SDL_FreeSurface(m_pImage);

    m_pTexture = nullptr;
    m_pImage = nullptr;
}

bool Texture::Load(const string& FileName)
{
    m_FileName = FileName;
    m_pImage = IMG_Load(("../Data/" + FileName).c_str());

    if (!m_pImage)
    {
        printf("Texture could not be loaded! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pImage);

    if (SDL_QueryTexture(m_pTexture, nullptr, nullptr, &m_Size.x, &m_Size.y) != 0)
    {
        printf("Texture error! SDL_Error: %s\n", SDL_GetError());
        throw 1;
    }

    return true;
}

void Texture::Display(vec2i Position, optional<vec2i> Size)const
{
    //int value = 0;
    //if( w.has_value() )
    //    value = w.value();
    
    //if (w)
    // value = *w;

    vec2i tmp = Size.value_or(m_Size);

    SDL_Rect Rect = { Position.x, Position.y, tmp.x, tmp.y };
    SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, &Rect);
}

void Texture::FreeResources()
{
    Texture::~Texture();
}

vec2i Texture::GetSize()
{
    if (m_pTexture)
    {
        return m_Size;
    }
    else
    {
        printf("Texture error! \n");
        throw 1;
    }

    return m_Size;
}

string Texture::GetName()
{
    return m_FileName;
}