#pragma once
#include "Common.h"
#include "vec2.h"

class Texture
{
public:
    Texture(sf::RenderWindow* pRenderer);
    ~Texture();
    bool Load(const string& FileName);
    void Display(vec2i Position, DisplayParameters Param = {})const;
    void FreeResources();
    vec2i GetSize();
    string GetName();

private:
    sf::RenderWindow* m_pRenderer = nullptr;
    sf::Texture       m_Texture;
    vec2i             m_Size;
    string            m_FileName;
};

