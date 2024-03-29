#pragma once
#include "Common.h"
#include "GameState.h"
#include "Sound.h"
#include "Texture.h"
#include "Definition.h"
#include "Timer.h"

class Engine
{
public:
    Engine()=default;
    ~Engine();
    static Engine* GetSingleton();
    bool Initialize();
    void Loop();
    void ChangeState(eStateID StateID);
    void ExitGame();
    void PlaySound(const string& FileName,float Volume = 1.0f);
    void DisplayTexture(TextureID ID, vec2 Position, DisplayParameters Param = {});
    // vec2i x - szerokosc tekstu, y - wysokosc linii w pionie
    vec2i DrawText(string Text, int PixelSize, vec2 Position, DrawTextParameters Param = {});
    void DestroyTextures();
    const Definition* FindDefinition(const string& FileName)const;
    void StartTimer(float Time, bool Loop, function<void()> Function, float Delay=0.f);
    void StopAllTimers();
    TextureID GenerateTextureID(string Name);

    shared_ptr<Texture> GetTexture(TextureID ID)const;
    vec2i GetTextureSize(TextureID ID)const;
    vec2i GetMousePos()const;
    sf::Window& GetWindow() { return m_Renderer; }
    const sf::Window& GetWindow()const { return m_Renderer; }

    // debug
    float GetFramesPerSecondValue()const;
    void SetFramesPerSecond(float Value);

private:
    bool LoadDefinition(const string& FileName);
    bool LoadAnimation(const string& FileName);

    using TexturesVec = vector<shared_ptr<Texture>>;
    using TexturesIDsMap = map<string, TextureID>;

    static Engine*                 pSingleton;
    sf::RenderWindow               m_Renderer;

    GameState*                     m_pCurrentState = nullptr;
    vector<unique_ptr<GameState>>  m_AllStates;
    vector<shared_ptr<Sound>>      m_LoadedSounds;
    mutable TexturesVec            m_LoadedTextures;
    mutable TexturesIDsMap         m_TexturesIDs;
    vector<pair<string, sf::Font>> m_Fonts;
    vector<unique_ptr<Definition>> m_Definitions;

    TimerManager                   m_TimerManager;
    float                          m_FramesPerSec = 60.0f;
};

