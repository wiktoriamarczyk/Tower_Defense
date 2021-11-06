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
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
    SDL_CloseAudio();
}

bool Engine::Initialize()
{
    srand(time(NULL));

    pSingleton = this;

    // zainicjalizowanie okna oraz dzwieku
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // zainicjalizowanie obrazkow
    if (!IMG_Init(IMG_INIT_PNG))
    {
        printf("IMG could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        return false;
    }

    //Mix_Volume(-1, 16);

    // utworzenie okna
    m_pWindow = SDL_CreateWindow("Space Invaders", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if (m_pWindow == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }


    // utworzenie renderera
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (m_pWindow == nullptr)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // stworzenie czcionki
    shared_ptr<Font> MyFont = make_shared<Font>();
    MyFont->LoadFont("../Data/FontData.txt");

    // dodanie wszystkich stanow gry do wektora
    m_AllStates.push_back(make_unique<InGameState>(MyFont, m_pRenderer));
    m_AllStates.push_back(make_unique<MainMenuState>(MyFont, m_pRenderer));

    // pierwszym stanem jest Menu gry
    ChangeState(eStateID::MAINMENU);

    return true;
}

void Engine::Loop()
{
    while (m_IsRunning)
    {
        SDL_Event EVENT = {};
        while (SDL_PollEvent(&EVENT))
        {
            if (EVENT.type == SDL_QUIT)
                return;

            if (EVENT.type == SDL_KEYDOWN)
            {
                m_pCurrentState->OnKeyDown(EVENT.key.keysym.scancode);
            }
        }

        SDL_Delay(1000 / 60);

        m_pCurrentState->Update(1.0f / 60.0f);
        m_pCurrentState->Render();

        // domyslnie nastepny stan jest UNKNOWN, gdy nie chcemy przechodzic do nowego stanu, zatem jesli jest tam cos innego, tzn. ze bylo zazadanie zmiany stanu
        if (m_pCurrentState->GetNextStateID() != eStateID::UNKNOWN)
        {
            SDL_Delay(100);
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
    m_IsRunning = false;
}

void Engine::PlaySound(const string& FileName,float Volume )
{
    for (int i = 0; i < m_LoadedSounds.size(); ++i)
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
    m_LoadedSounds.back()->Play();
}

vec2 Engine::GetMousePos() const
{
    int x = 0, y = 0;
    SDL_GetMouseState(&x, &y);
    return vec2(float(x),float(y));
}
