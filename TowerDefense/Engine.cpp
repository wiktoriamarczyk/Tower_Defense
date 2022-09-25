#include "Engine.h"
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
    ZoneScoped;

    srand(time(NULL));

    sf::err().rdbuf(std::cout.rdbuf());

    pSingleton = this;

    // stworzenie okna
    m_Renderer.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Tower Defense" );//, sf::Style::None );

    // inicjalizacja czcionek
    sf::Font font;
    font.loadFromFile("../Data/Fonts/Graph-35-pix.ttf");
    m_Fonts.push_back(make_pair(string("Graph-35-pix"), font));

    font.loadFromFile("../Data/Fonts/NotoEmoji-Regular.ttf");
    m_Fonts.push_back(make_pair(string("NotoEmoji-Regular"), font));

    // zaladowanie definicji
    LoadDefinition("/Definitions/Dragon.xml");
    LoadDefinition("/Definitions/Basilisk.xml");
    LoadDefinition("/Definitions/Tower1.xml");
    LoadDefinition("/Definitions/Tower2.xml");
    LoadDefinition("/Definitions/Tower3.xml");

    // zaladowanie animacji obiektow
    LoadAnimation("/Textures/DragonAnim.xml");
    LoadAnimation("/Textures/DragonDeathAnim.xml");
    LoadAnimation("/Textures/DragonHitAnim.xml");
    LoadAnimation("/Textures/BasiliskAnim.xml");
    LoadAnimation("/Textures/BasiliskDeathAnim.xml");
    LoadAnimation("/Textures/BasiliskHitAnim.xml");
    LoadAnimation("/Textures/Tower3Anim.xml");

    // dodanie wszystkich stanow gry do wektora
    m_AllStates.push_back(make_unique<InGameState>());
    m_AllStates.push_back(make_unique<MainMenuState>());

    // pierwszym stanem jest Menu gry
    ChangeState(eStateID::MAINMENU);

    return true;
}

void Engine::Loop()
{
    while (m_Renderer.isOpen())
    {
        FrameMark;
        ZoneScoped;

        sf::Clock clock;

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

        for (int i = 0; i < m_LoadedTextures.size(); ++i)
        {
            m_LoadedTextures[i]->Update(1.0f / m_FramesPerSec);
        }

        m_pCurrentState->Update(1.0f / m_FramesPerSec);
        m_pCurrentState->Render(m_Renderer);
        m_TimerManager.Update(1.0f / m_FramesPerSec);

        auto timePassed = clock.getElapsedTime();

        sf::sleep(sf::milliseconds(1000 / 60) - timePassed);

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
    ZoneScoped;

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
    ZoneScoped;

    for (int i = 0; i < m_LoadedTextures.size(); ++i)
    {
        if (m_LoadedTextures[i]->GetName() == FileName)
        {
            return m_LoadedTextures[i];
        }
    }

    shared_ptr<Texture> tmpTexture = make_shared<Texture>();

    if (!tmpTexture->Load(FileName))
    {
        return nullptr;
    }

    m_LoadedTextures.push_back(tmpTexture);
    return tmpTexture;
}

void Engine::DisplayTexture(const string& FileName, vec2 Position, DisplayParameters Param)
{
    ZoneScoped;

    // jesli znalezlismy teksture, wyswietl ja
    if (auto pTexture = GetTexture(FileName))
    {
        pTexture->Display(m_Renderer, Position, Param);
    }
}

vec2i Engine::DrawText(string TextUtf8, int PixelSize, vec2 InputPosition, DrawTextParameters Param)
{
    ZoneScoped;

    sf::Font* pFont = nullptr;

    static vec2 LastLineEnd  = vec2{0,0};
    static vec2 lastLineSize = vec2{0,0};

    vec2i Position = vec2i(InputPosition);

    for (size_t i = 0; i < m_Fonts.size(); ++i)
    {
        if (m_Fonts[i].first == Param.FontName)
            pFont = &m_Fonts[i].second;
    }

    if (!pFont)
        return vec2i(0, 0);

    sf::Text text;
    text.setFont(*pFont);
    text.setString( sf::String::fromUtf8( TextUtf8.begin() , TextUtf8.end() ) );
    text.setCharacterSize(PixelSize);
    text.setStyle(static_cast<sf::Text::Style>(Param.FontStyle));

    auto bounds = text.getLocalBounds();
    bounds.height += pFont->getLineSpacing(text.getCharacterSize());

    if (!Param.DisableDraw)
    {
        int horizontalShift = 0;
        text.setColor(Param.FontColor);

        switch (Param.HAlign)
        {
        case eTextHorizontalAlign::LEFT: horizontalShift = 0;
            break;
        case eTextHorizontalAlign::CENTER: horizontalShift = -bounds.width / 2;
            break;
        case eTextHorizontalAlign::RIGHT: horizontalShift = -bounds.width;
        }

        text.setPosition(vec2(Position) + vec2(horizontalShift, 0));

        // work in progress... / 4 to cheat
        if( Param.ContinueLastLine )
        {
            text.setPosition(LastLineEnd - vec2(0,(bounds.height-lastLineSize.y)/4));
            //if( bounds.height > lastLineHeight )
            //    bounds.height = bounds.height - lastLineHeight;
            //else

            bounds.height = 0;
        }

        m_Renderer.draw(text);

        LastLineEnd  = vec2(text.getPosition()) + vec2(bounds.width,0);
        lastLineSize = vec2{bounds.width, bounds.height};
    }


    return vec2i(bounds.width, bounds.height);
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
    ZoneScoped;

    for (size_t i = 0; i < m_Definitions.size(); ++i)
    {
        if (m_Definitions[i]->GetName() == FileName)
            return m_Definitions[i].get();
    }

    return nullptr;
}

void Engine::StartTimer(float Time, bool Loop, function<void()> Function, float Delay)
{
    ZoneScoped;
    m_TimerManager.StartTimer(Time, Loop, Function, Delay);
}

void Engine::StopAllTimers()
{
    m_TimerManager.StopAllTimers();
}

vec2i Engine::GetTextureSize(const string& FileName)const
{
    ZoneScoped;

    if (auto pTexture = GetTexture(FileName))
    {
        return pTexture->GetSize();
    }
    else
        return vec2i(0,0);
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
    ZoneScoped;

    unique_ptr<Definition> pDef = make_unique<Definition>();

    if (!pDef->LoadFromFile(FileName))
        return false;

    m_Definitions.push_back(move(pDef));

    return true;
}

bool Engine::LoadAnimation(const string& FileName)
{
    ZoneScoped;

    if (!LoadDefinition(FileName))
       return false;

    auto pDef = FindDefinition(FileName);
    vector<shared_ptr<Texture>> unitAnimationFrames;
    vector<string> textureNames = pDef->GetStringValuesVector("Frame");

    for (size_t i = 0; i < textureNames.size(); ++i)
    {
        unitAnimationFrames.push_back(GetTexture(textureNames[i]));
    }

    shared_ptr<AnimatedTexture> unitAnimationTexture = make_shared<AnimatedTexture>();
    unitAnimationTexture->Load(unitAnimationFrames, FileName, pDef->GetAttributeFloatValue("FrameSpeed",12.0f));
    m_LoadedTextures.push_back(unitAnimationTexture);

   return true;
}