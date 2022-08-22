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

    m_Size = m_Texture.getSize();

    return true;
}

void Texture::Display(vec2 Position, DisplayParameters Param)const
{
    vec2 finalSize = vec2(m_Size) * Param.DrawScale;
    vec2 finalPos = Position - finalSize * Param.Pivot;

    sf::Sprite sprite;
    sprite.setTexture(m_Texture);
    sprite.setPosition(finalPos);
    sprite.setScale(Param.DrawScale);
    sprite.setColor(Param.DrawColor);
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

vec2i Texture::GetSize()
{
    return m_Size;
}

string Texture::GetName()
{
    return m_FileName;
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
    if( m_Frames.size() )
        m_Size = m_Frames[0]->GetSize();
}
