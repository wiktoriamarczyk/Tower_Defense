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

    SetSize(m_Texture.getSize());

    return true;
}

void Texture::Display(vec2 Position, DisplayParameters Param)const
{
    sf::Sprite sprite;
    sprite.setTexture(m_Texture);
    sprite.setPosition(Position);
    sprite.setScale(Param.DrawScale);
    sprite.setColor(Param.DrawColor);
    sprite.setOrigin(Param.Pivot*GetSize());
    sprite.setRotation(Param.Rotation);

    sf::RenderStates states;

    if (Param.DrawMode == eDrawMode::ADDITIVE)
        states.blendMode = sf::BlendAdd;

    m_pRenderer->draw(sprite, states);
}

void Texture::FreeResources()
{
    Texture::~Texture();
}

void AnimatedTexture::Display(vec2 Position, DisplayParameters Param) const
{
    m_Frames[(int)m_CurrentFrame]->Display(Position, Param);
}

void AnimatedTexture::Update(float DeltaTime)
{
    m_CurrentFrame += DeltaTime * m_FrameSpeed;

    if (m_CurrentFrame >= m_Frames.size())
        m_CurrentFrame = 0;
}

void AnimatedTexture::Load(vector<shared_ptr<Texture>> Frames, const string& AnimatedTxtName)
{
    m_Frames = Frames;
    m_FileName = AnimatedTxtName;

    // ustaw rozmiar animowanej tekstury jako rozmiar z 1 klatki
    if(m_Frames.size())
        SetSize(m_Frames[0]->GetSize());
}

vec2i Texture::GetSize()const
{
    return m_Size;
}

string Texture::GetName()const
{
    return m_FileName;
}

void Texture::SetSize(vec2i Size)
{
    m_Size = Size;
}
