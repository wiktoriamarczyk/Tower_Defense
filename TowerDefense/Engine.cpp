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

    /*if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        return false;
    }*/

    // stworzenie czcionki
    shared_ptr<Font> MyFont = make_shared<Font>();
    MyFont->LoadFont("../Data/FontData.txt");

    // dodanie wszystkich stanow gry do wektora
    m_AllStates.push_back(make_unique<InGameState>(MyFont));
    m_AllStates.push_back(make_unique<MainMenuState>(MyFont));

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

        m_pCurrentState->Update(1.0f / 60.0f);
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

    shared_ptr<Texture> temp_texture = make_shared<Texture>(const_cast<sf::RenderWindow*>(&m_Renderer));

    if (!temp_texture->Load(FileName))
    {
        return nullptr;
    }

    m_LoadedTextures.push_back(temp_texture);
    return temp_texture;
}

void Engine::DisplayTexture(const string& FileName, vec2i Position, optional<vec2i> Size)
{
    // jesli znalezlismy teksture, wyswietl ja
    if (auto pTexture = GetTexture(FileName))
    {
        pTexture->Display(Position, Size);
    }
}

void Engine::DestroyTextures()
{
    for (int i = 0; i < m_LoadedTextures.size(); ++i)
    {
        m_LoadedTextures[i]->FreeResources();
    }
}

vec2i Engine::GetTextureSize(const string& FileName)const
{
    if (auto pTexture = GetTexture(FileName))
    {
        return pTexture->GetSize();
    }
    else vec2i(0,0);
}

vec2i Engine::GetMousePos() const
{
    return sf::Mouse::getPosition();
}
