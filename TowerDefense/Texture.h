#pragma once
#include "Common.h"
#include "vec2.h"

class Texture
{
public:
    Texture(SDL_Renderer* pRenderer);
    ~Texture();
    bool Load(const string& FileName);
    void Display(int x, int y, int w, int h)const;
    void FreeResources();
    vec2i GetTextureSize();
    string GetName();

private:
    SDL_Renderer* m_pRenderer = nullptr;
    SDL_Surface*  m_pImage = nullptr;
    SDL_Texture*  m_pTexture = nullptr;
    vec2i         m_TextureSize;
    string        m_FileName;
};

