#pragma once
#include "GameObject.h"
#include "Common.h"

struct ParticleData
{
    // pozycja partikla w przestrzeni ParticleEmiter (pozycja partikla na ekranie to m_ParticlePosition + ParticleEmiter.m_Position)
    vec2 m_ParticlePosition;
    // wektor kierunek ruchu - jego długosc to dodatkowo prędkość na sekundę
    vec2 m_DirectionVector;
    // pozostały czas życia (ile sekund mu zostało)
    float m_LifeTime = INFINITY;
    // początkowy czas "pauzy" - przez ten czas partikiel nic nie robi ani się nie rysuje
    float m_StartDelayTime = 0;
    // skala partikla
    float m_Scale = 1.0f;
    // kolor partikla
    sf::Color m_Color = sf::Color::Red;
    // nazwa tekstury
    const char* m_Texture = "";

    float m_Rotation = 0.0f;
    float m_RotationSpeed = 0.0f;
};

class ParticleEmiter : public GameObject
{
public:
    ParticleEmiter(string FileName, int ParticleCount, float Scale, float MaxLifeTime=INFINITY);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    void InitializeTarget(shared_ptr<GameObject> Target);

private:
    void UpdateState(float DeltaTime);

    vector<ParticleData>   m_Particles;
    string                 m_FileName;
    shared_ptr<GameObject> m_Target = nullptr;
};