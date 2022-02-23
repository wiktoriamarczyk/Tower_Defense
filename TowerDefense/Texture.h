#pragma once
#include "Common.h"
#include "vec2.h"

class Texture
{
public:
    Texture(SDL_Renderer* pRenderer);
    ~Texture();
    bool Load(const string& FileName);
    void Display(int x, int y, optional<int> w, optional<int> h)const;
    void FreeResources();
    vec2i GetSize();
    string GetName();

private:
    SDL_Renderer* m_pRenderer = nullptr;
    SDL_Surface*  m_pImage = nullptr;
    SDL_Texture*  m_pTexture = nullptr;
    vec2i         m_Size;
    string        m_FileName;
};

