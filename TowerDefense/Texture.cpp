#include "Texture.h"

bool Texture::Load(const string& FileName)
{
    ZoneScoped;
    m_FileName = FileName;

    if (!m_Texture.loadFromFile("../Data/" + FileName))
    {
        printf("Texture could not be loaded!\n");
        return false;
    }

    SetSize(m_Texture.getSize());

    return true;
}

void Texture::Display(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Param)const
{
    ZoneScoped;

    sf::Sprite sprite;
    sprite.setTexture(m_Texture);
    sprite.setPosition(Position);
    sprite.setScale(Param.DrawScale);
    sprite.setColor(Param.DrawColor);
    sprite.setOrigin(Param.Pivot * GetSize());
    sprite.setRotation(Param.Rotation);

    sf::RenderStates states;

    if (Param.DrawMode == eDrawMode::ADDITIVE)
        states.blendMode = sf::BlendAdd;

    Renderer.draw(sprite, states);
}

void Texture::FreeResources()
{
    Texture::~Texture();
}

void AnimatedTexture::Display(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Param) const
{
    ZoneScoped;
    if (Param.CurrentFrame > -1 && Param.CurrentFrame < m_Frames.size())
        m_Frames[Param.CurrentFrame]->Display(Renderer, Position, Param);
    else
        m_Frames[(uint32_t)m_CurrentFrame]->Display(Renderer, Position, Param);
}

void AnimatedTexture::Update(float DeltaTime)
{
    ZoneScoped;
    m_CurrentFrame += DeltaTime * m_FrameSpeed;

    if (m_CurrentFrame >= m_Frames.size())
        m_CurrentFrame = 0;
}

void AnimatedTexture::Load(vector<shared_ptr<Texture>> Frames, const string& AnimatedTxtName,float FrameSpeed)
{
    ZoneScoped;
    m_Frames = Frames;
    m_FileName = AnimatedTxtName;
    m_FrameSpeed = FrameSpeed;

    // ustaw rozmiar animowanej tekstury jako rozmiar z 1 klatki
    if(m_Frames.size())
        SetSize(m_Frames[0]->GetSize());
}

uint32_t AnimatedTexture::GetFrmaesCount() const
{
    return uint32_t(m_Frames.size());
}

float AnimatedTexture::GetFrameSpeed() const
{
    return m_FrameSpeed;
}

vec2i Texture::GetSize()const
{
    return m_Size;
}

string Texture::GetName()const
{
    return m_FileName;
}

const sf::Texture& Texture::GetSFMLTexture()const
{
    return m_Texture;
}

void Texture::SetSize(vec2i Size)
{
    m_Size = Size;
}

uint32_t Texture::GetFrmaesCount() const
{
    return 1;
}

float Texture::GetFrameSpeed() const
{
    return 12.0f;
}