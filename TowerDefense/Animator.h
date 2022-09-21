#pragma once
#include "Common.h"

class Animator
{
public:
    void AddTexture(eTextureType Type, const string& FileName);
    void DisplayTexture(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Parameters);
    bool StartAnimation(eTextureType Type, bool Loop, function<void()> FinishCallback={});
    void Update(float DeltaTime);
    void SetFrameSpeed(float FrameSpeed);
private:
    struct TextureData
    {
        eTextureType m_Type;
        string m_FileName;
        uint32_t m_FramesCount;
    };
    vector<TextureData>                 m_TextureNames;
    TextureData                         m_CurrentTexture;
    bool                                m_Loop = false;
    bool                                m_Working = false;
    function<void()>                    m_FinishCallback;
    float                               m_CurrentFrame = 0; //< numer klatki, ktory w display zostanie przyciety do inta
    float                               m_FrameSpeed = 12.f; //< ile klatek animacji zostanie wyswietlonych w ciagu 1s
};