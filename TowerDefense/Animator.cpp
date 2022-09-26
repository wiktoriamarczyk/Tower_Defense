#include "Animator.h"
#include "Engine.h"

void Animator::AddTexture(eTextureType Type, const string& FileName )
{
    auto id = Engine::GetSingleton()->GenerateTextureID(FileName);
    auto pTexture = Engine::GetSingleton()->GetTexture(id);
    if (!pTexture)
        return;

    TextureData Data;
    Data.m_Type = Type;
    Data.m_FileName = FileName;
    Data.m_ID = id;
    Data.m_FramesCount = pTexture->GetFrmaesCount();
    Data.m_FrameSpeed = pTexture->GetFrameSpeed();
    m_TextureNames.push_back(Data);
}

void Animator::DisplayTexture(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Parameters)
{
    Parameters.CurrentFrame = (int32_t)m_CurrentFrame;
    Engine::GetSingleton()->DisplayTexture(m_CurrentTexture.m_ID, Position , Parameters);
}

bool Animator::StartAnimation(eTextureType Type, bool Loop, function<void()> FinishCallback, bool AllowInterrupt)
{
    if (m_CurrentTexture.m_Type == Type && !AllowInterrupt)
    {
        return true;
    }

    m_Loop = Loop;
    m_FinishCallback = FinishCallback;
    m_CurrentFrame = 0;
    m_Working = true;

    for (size_t i = 0; i < m_TextureNames.size(); ++i)
    {
        if (m_TextureNames[i].m_Type == Type)
        {
            m_CurrentTexture = m_TextureNames[i];
            return true;
        }
    }
    return false;
}

void Animator::Update(float DeltaTime)
{
    if (!m_Working)
        return;

    m_CurrentFrame += DeltaTime * m_CurrentTexture.m_FrameSpeed;

    if (m_CurrentFrame >= m_CurrentTexture.m_FramesCount)
    {
        if (m_Loop)
        {
            m_CurrentFrame = 0;
        }
        else
        {
            m_Working = false;
            m_FinishCallback();
        }
    }
}

void Animator::SetFrameSpeed(float FrameSpeed)
{
    m_CurrentTexture.m_FrameSpeed = FrameSpeed;
}