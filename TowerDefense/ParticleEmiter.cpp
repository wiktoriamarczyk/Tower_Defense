#include "ParticleEmiter.h"
#include "Engine.h"

// ParticleCount - liczba partikli jaką stworzymy
ParticleEmiter::ParticleEmiter(string FileName, int ParticleCount, float Scale, float MaxLifeTime)
{
    m_FileName = FileName;

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
        float ParticleSpeed = GetRandFloat(80.0f, 130.0f);
        // wektor kierunku, ktorego dlugosc wyznacza predkosc poruszania particla na sekunde (dlatego przemnazamy przez particlespeed)
        tmp.m_DirectionVector = Vec2Dir * ParticleSpeed;
        // losujemy skalę
        tmp.m_Scale = GetRandFloat(Scale/2.f, Scale);
        // przypisujemy nazwę tekstury do partikla
        tmp.m_Texture = m_FileName.c_str();

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

        // zmiana pozycji partikli
        if (!m_Target)
        {
            m_Particles[i].m_ParticlePosition += m_Particles[i].m_DirectionVector * DeltaTime;
            m_Particles[i].m_LifeTime         -= DeltaTime;
        }
        else
        {
            m_Particles[i].m_ParticlePosition = m_Target->GetPosition();
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

    if (!m_Target->GetLifeStatus())
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
        const vec2   ParticlePos     = m_Particles[i].m_ParticlePosition + GetPosition();
        sf::Color    ParticleColor   = m_Particles[i].m_Color * sf::Color::Red;
        const string ParticleName    = m_Particles[i].m_Texture;


        Engine::GetSingleton()->DisplayTexture(ParticleName, ParticlePos, { .DrawMode = eDrawMode::ADDITIVE , .DrawScale = ParticleScale , 
                                                                            .Pivot = vec2(0.5f,0.5f), .DrawColor = ParticleColor, 
                                                                            .Rotation = ParticleRotation });
    }
}

void ParticleEmiter::InitializeTarget(shared_ptr<GameObject> Target)
{
    m_Target = Target;
}