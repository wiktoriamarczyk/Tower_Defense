#pragma once
#include "Common.h"

class Animator
{
public:
    void AddTexture(eTextureType Type, const string& FileName);
    void DisplayTexture(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Parameters);
    bool StartAnimation(eTextureType Type, bool Loop, function<void()> FinishCallback={}, bool AllowInterrupt=false);
    void Update(float DeltaTime);
    void SetFrameSpeed(float FrameSpeed);
private:
    struct TextureData
    {
        eTextureType m_Type;
        string       m_FileName;
        TextureID    m_ID = TextureID::INVALID;
        uint32_t     m_FramesCount;
        float        m_FrameSpeed = 12.0f; //< ile klatek animacji zostanie wyswietlonych w ciagu 1s
    };
    vector<TextureData>                 m_TextureNames;
    TextureData                         m_CurrentTexture;
    bool                                m_Loop = false;
    bool                                m_Working = false;
    function<void()>                    m_FinishCallback;
    float                               m_CurrentFrame = 0; //< numer klatki, ktory w display zostanie przyciety do inta
};