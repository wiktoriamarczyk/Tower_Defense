#include "ParticleEmiter.h"
#include "Engine.h"

// ParticleCount - liczba partikli jaką stworzymy
ParticleEmiter::ParticleEmiter(string FileName, int ParticleCount, float Scale, float MaxLifeTime)
{
    m_FileName = FileName;
    m_ID = Engine::GetSingleton()->GenerateTextureID(FileName);

    // kąt pomiędzy poszczególnymi partiklami -> pełne koło / liczba partikli
    float AngleDistance = 3.14f * 2 / ParticleCount;

    // tworzymy poszczególne partikle
    for (int i = 0; i < ParticleCount; ++i)
    {
        ParticleData tmp;

        // kąt pod jakim leci ten partikiel
        float Angle = AngleDistance * i;
        // uwtwórz wektor jednostkowy kierunku z kąta używając:
        //
        //         /|
        //        / |
        //    Z  /  |
        //      /   | X
        //     /    |
        //    / a)  |
        //    -------
        //      Y
        //
        // a - kąt
        // Z = 1 -> wektor ma długość 1
        // sin(a) = X/Z ->  X = sin(a)
        // cos(a) = Y/Z ->  Y = cos(a)

        vec2 Vec2Dir(sin(Angle), cos(Angle));

        // losujemy prędkość pomiędzy 80 a 130 pixeli na sekundę
        float ParticleSpeed = 1;//GetRandFloat(80.0f, 130.0f);
        // wektor kierunku, ktorego dlugosc wyznacza predkosc poruszania particla na sekunde (dlatego przemnazamy przez particlespeed)
        tmp.m_DirectionVector = Vec2Dir * ParticleSpeed;
        // losujemy skalę
        tmp.m_Scale = GetRandFloat(Scale/2.f, Scale);
        // przypisujemy nazwę tekstury do partikla
        tmp.m_Texture = m_FileName.c_str();
        tmp.m_TextureID = m_ID;

        tmp.m_ParticlePosition = vec2(GetRandFloat(-10,10), GetRandFloat(-10,10));

        if (MaxLifeTime != INFINITY)
        {
            // losujemy czas życia w sekundach
            tmp.m_LifeTime = GetRandFloat(MaxLifeTime / 2.f, MaxLifeTime);
            // losujemy czas "opóźnienia" - czas na początku w którym partikle nic nie robi i się nie renderuje
            tmp.m_StartDelayTime = GetRandFloat(0.0f, 0.15f);
        }

        // wrzucamy dane partikla do wektora
        m_Particles.push_back(tmp);
    }
}

void ParticleEmiter::Update(float DeltaTime)
{
    UpdateState(DeltaTime);
}

void ParticleEmiter::UpdateState(float DeltaTime)
{
    m_TotalTime += DeltaTime;

    // rozmiar losowej 'wibracji' partikla
    float WiggleSize = 10;

    for (size_t i = 0; i < m_Particles.size(); ++i)
    {
        // póki mamy jakiś czas "opóźnienia" to go zmiejszammy i nie robimy nic innego
        if (m_Particles[i].m_StartDelayTime > 0)
        {
            m_Particles[i].m_StartDelayTime -= DeltaTime;
            continue;
        }

        // rotacja partikli
        m_Particles[i].m_Rotation             += m_Particles[i].m_RotationSpeed * DeltaTime;

        if (m_Target)
            SetPosition(m_Target->GetPosition() + m_Target->GetSize() / 2 * vec2(0.1f, 0.1f));

        m_Particles[i].m_LifeTime         -= DeltaTime;

        // zmiana pozycji partikli
        m_Particles[i].m_ParticlePosition += m_Particles[i].m_DirectionVector * DeltaTime;

        if (WiggleSize > 0.0f)
        {
            float SomeTime = (i*0.3f + m_TotalTime)*(25/WiggleSize);

            // pseudo-losowy ruch partikli - skladany z 3 sin/cos o roznej amplitudzie i okresie
            m_Particles[i].m_OptionalOffset.x = (sin(SomeTime*4)*0.4f+sin(SomeTime*5)*0.28f+sin(SomeTime*3)*0.32f)*WiggleSize;
            m_Particles[i].m_OptionalOffset.y = (cos(SomeTime*5)*0.4f+cos(SomeTime*3)*0.28f+cos(SomeTime*5)*0.32f)*WiggleSize;
        }

        // smierc poszczegolnych partikli
        if (m_Particles[i].m_LifeTime <= 0)
        {
            m_Particles.erase(m_Particles.begin() + i);
            --i;
        }
    }

    // smierc emitera
    if (m_Particles.empty())
    {
        SetLifeStatus(false);
    }

    if (m_Target && !m_Target->GetLifeStatus())
    {
        SetLifeStatus(false);
    }
}

void ParticleEmiter::Render(sf::RenderWindow& Renderer)
{
    vec2 Scale = vec2(2, 2);
    vec2i Size = vec2i(32, 32);

    for (size_t i = 0; i < m_Particles.size(); ++i)
    {
        // póki mamy jakiś czas "opóźnienia" to się nie rysujemy
        if (m_Particles[i].m_StartDelayTime > 0)
            continue;

        const vec2   ParticleScale   = Scale * m_Particles[i].m_Scale;
        const float  ParticleRotation= m_Particles[i].m_Rotation;
        const vec2   ParticlePos     = m_Particles[i].m_ParticlePosition + m_Particles[i].m_OptionalOffset + GetPosition();
        sf::Color    ParticleColor   = m_Particles[i].m_Color;
        const TextureID ParticleID   = m_Particles[i].m_TextureID;


        Engine::GetSingleton()->DisplayTexture(ParticleID, ParticlePos, { .DrawMode = eDrawMode::ADDITIVE , .DrawScale = ParticleScale ,
                                                                            .Pivot = vec2(0.5f,0.5f), .DrawColor = ParticleColor,
                                                                            .Rotation = ParticleRotation });
    }
}

void ParticleEmiter::InitializeTarget(shared_ptr<GameObject> Target)
{
    m_Target = Target;
}