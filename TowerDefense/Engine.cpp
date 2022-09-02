#include "Engine.h"
#include "Font.h"
#include "InGameState.h"
#include "MainMenuState.h"

Engine* Engine::pSingleton = nullptr;

Engine* Engine::GetSingleton()
{
    return pSingleton;
}

Engine::~Engine()
{
    pSingleton = nullptr;
}

bool Engine::Initialize()
{
    srand(time(NULL));

    sf::err().rdbuf(std::cout.rdbuf());

    pSingleton = this;

    // stworzenie okna
    m_Renderer.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Tower Defense");

    // stworzenie czcionki
    shared_ptr<Font> myFont = make_shared<Font>();
    myFont->LoadFont("../Data/FontData.txt");

    // zaladowanie definicji
    LoadDefinition("Dragon.xml");
    LoadDefinition("Basilisk.xml");
    LoadDefinition("Tower1.xml");
    LoadDefinition("Tower2.xml");
    LoadDefinition("Tower3.xml");

    // zaladowanie animacji obiektow
    LoadAnimation("DragonAnim.xml");
    LoadAnimation("BasiliskAnim.xml");
    LoadAnimation("Tower3Anim.xml");

    // dodanie wszystkich stanow gry do wektora
    m_AllStates.push_back(make_unique<InGameState>(myFont));
    m_AllStates.push_back(make_unique<MainMenuState>(myFont));

    // pierwszym stanem jest Menu gry
    ChangeState(eStateID::INGAME);

    return true;
}

void Engine::Loop()
{
    while (m_Renderer.isOpen())
    {
        sf::Event event;
        while (m_Renderer.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_Renderer.close();

            if (event.type == sf::Event::KeyPressed)
            {
                m_pCurrentState->OnKeyDown(event.key.code);
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                m_pCurrentState->OnMouseButtonDown(event.mouseButton.button);
            }
        }
        
        sf::sleep(sf::milliseconds(1000 / 60));

        for (int i = 0; i < m_LoadedTextures.size(); ++i)
        {
            m_LoadedTextures[i]->Update(1.0f / m_FramesPerSec);
        }

        m_pCurrentState->Update(1.0f / m_FramesPerSec);
        m_pCurrentState->Render(m_Renderer);

        // domyslnie nastepny stan jest UNKNOWN, gdy nie chcemy przechodzic do nowego stanu, zatem jesli jest tam cos innego, tzn. ze bylo zazadanie zmiany stanu
        if (m_pCurrentState->GetNextStateID() != eStateID::UNKNOWN)
        {
            sf::sleep(sf::milliseconds(100));
            ChangeState(m_pCurrentState->GetNextStateID());
        }
    }
}

void Engine::ChangeState(eStateID StateID)
{
    for (int i = 0; i < m_AllStates.size(); ++i)
    {
        if (m_AllStates[i]->GetStateID() == StateID)
        {
            m_pCurrentState = m_AllStates[i].get();
            m_pCurrentState->OnEnter();
            return;
        }
    }
}

void Engine::ExitGame()
{
    m_Renderer.close();
}

void Engine::PlaySound(const string& FileName,float Volume )
{
    /*for (int i = 0; i < m_LoadedSounds.size(); ++i)
    {
        if (m_LoadedSounds[i]->GetName() == FileName)
        {
            m_LoadedSounds[i]->Play();
            return;
        }
    }
    shared_ptr<Sound> temp_sound = make_shared<Sound>();
    temp_sound->Load(FileName,Volume);
    m_LoadedSounds.push_back(temp_sound);
    m_LoadedSounds.back()->Play();*/
}

shared_ptr<Texture> Engine::GetTexture(const string& FileName)const
{
    for (int i = 0; i < m_LoadedTextures.size(); ++i)
    {
        if (m_LoadedTextures[i]->GetName() == FileName)
        {
            return m_LoadedTextures[i];
        }
    }

    shared_ptr<Texture> tmpTexture = make_shared<Texture>(const_cast<sf::RenderWindow*>(&m_Renderer));

    if (!tmpTexture->Load(FileName))
    {
        return nullptr;
    }

    m_LoadedTextures.push_back(tmpTexture);
    return tmpTexture;
}

void Engine::DisplayTexture(const string& FileName, vec2 Position, DisplayParameters Param)
{
    // jesli znalezlismy teksture, wyswietl ja
    if (auto pTexture = GetTexture(FileName))
    {
        pTexture->Display(Position, Param);
    }
}

void Engine::DestroyTextures()
{
    for (int i = 0; i < m_LoadedTextures.size(); ++i)
    {
        m_LoadedTextures[i]->FreeResources();
    }
}

const Definition* Engine::FindDefinition(const string& FileName) const
{
    for (size_t i = 0; i < m_Definitions.size(); ++i)
    {
        if (m_Definitions[i]->GetName() == FileName)
            return m_Definitions[i].get();
    }

    return nullptr;
}

vec2i Engine::GetTextureSize(const string& FileName)const
{
    if (auto pTexture = GetTexture(FileName))
    {
        return pTexture->GetSize();
    }
    else 
        vec2i(0,0);
}

vec2i Engine::GetMousePos() const
{
    auto realWindowSize = GetWindow().getSize();
    vec2 SCREEN_RATIO = vec2(SCREEN_WIDTH / (float)realWindowSize.x, SCREEN_HEIGHT / (float)realWindowSize.y);
    return vec2i(int(sf::Mouse::getPosition(GetWindow()).x * SCREEN_RATIO.x), int(sf::Mouse::getPosition(GetWindow()).y *  SCREEN_RATIO.y));
}

float Engine::GetFramesPerSecondValue()const
{
    return m_FramesPerSec;
}

void Engine::SetFramesPerSecond(float Value)
{
    m_FramesPerSec = Value;
}

bool Engine::LoadDefinition(const string& FileName)
{
    unique_ptr<Definition> pDef = make_unique<Definition>();

    if (!pDef->LoadFromFile(FileName))
        return false;

    m_Definitions.push_back(move(pDef));

    return true;
}

bool Engine::LoadAnimation(const string& FileName)
{
    if (!LoadDefinition(FileName))
       return false;

    auto pDef = FindDefinition(FileName);
    vector<shared_ptr<Texture>> unitAnimationFrames;
    vector<string> textureNames = pDef->GetStringValuesVector("Frame");

    for (size_t i = 0; i < textureNames.size(); ++i)
    {
        unitAnimationFrames.push_back(GetTexture(textureNames[i]));
    }

    shared_ptr<AnimatedTexture> unitAnimationTexture = make_shared<AnimatedTexture>(&m_Renderer);
    unitAnimationTexture->Load(unitAnimationFrames, FileName);
    m_LoadedTextures.push_back(unitAnimationTexture);

   return true;
}