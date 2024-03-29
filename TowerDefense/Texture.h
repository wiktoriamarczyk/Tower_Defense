#pragma once
#include "Common.h"
#include "vec2.h"

class Texture
{
public:
    Texture()=default;
    ~Texture() {};
    bool Load(const string& FileName);
    virtual void Display(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Param = {})const;
    virtual void Update(float DeltaTime){};
    void FreeResources();

    vec2i GetSize()const;
    string GetName()const;
    const sf::Texture& GetSFMLTexture()const;

    void SetSize(vec2i Size);
    virtual uint32_t GetFrmaesCount()const;
    virtual float GetFrameSpeed()const;
protected:
    sf::RenderWindow* m_pRenderer = nullptr;
    vec2i             m_Size;
    string            m_FileName;

private:
    sf::Texture       m_Texture;
};

class AnimatedTexture : public Texture
{
public:
    void Display(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Param = {})const override;
    void Update(float DeltaTime);
    void Load(vector<shared_ptr<Texture>> Frames, const string& AnimatedTxtName,float FrameSpeed);

    uint32_t GetFrmaesCount()const override;
    float GetFrameSpeed()const override;
private:
    vector<shared_ptr<Texture>> m_Frames;
    float                       m_CurrentFrame = 0;  //< numer klatki, ktory w display zostanie przyciety do inta
    float                       m_FrameSpeed = 12.f; //< ile klatek animacji zostanie wyswietlonych w ciagu 1s

};