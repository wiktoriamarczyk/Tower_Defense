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

    if (SDL_QueryTexture(m_pTexture, nullptr, nullptr, &m_TextureSize.x, &m_TextureSize.y) != 0)
    {
        printf("Texture error! SDL_Error: %s\n", SDL_GetError());
        throw 1;
    }

    return true;
}

void Texture::Display(int x, int y, optional<int> w, optional<int> h)const
{
    //int value = 0;
    //if( w.has_value() )
    //    value = w.value();
    
    //if (w)
    // value = *w;

    SDL_Rect Rect = { x, y, w.value_or(m_TextureSize.x), h.value_or(m_TextureSize.y) };
    SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, &Rect);
}

void Texture::FreeResources()
{
    Texture::~Texture();
}

vec2i Texture::GetTextureSize()
{
    if (m_pTexture)
    {
        return m_TextureSize;
    }
    else
    {
        printf("Texture error! SDL_Error: %s\n");
        throw 1;
    }

    return m_TextureSize;
}

string Texture::GetName()
{
    return m_FileName;
}