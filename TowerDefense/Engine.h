#pragma once
#include "Common.h"
#include "GameState.h"
#include "Sound.h"
#include "Texture.h"

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
    shared_ptr<Texture> GetTexture(const string& FileName)const;
    void DisplayTexture(const string& FileName, int x, int y, optional<int> w = nullopt, optional<int> h = nullopt);
    void DestroyTextures();
    vec2i GetTextureSize(const string& FileName)const;
    vec2i GetMousePos()const;

private:
    using TexturesVec = vector<shared_ptr<Texture>>;

    bool                          m_IsRunning = true;
                                  
    static Engine*                pSingleton;
    SDL_Renderer*                 m_pRenderer = nullptr;
    SDL_Window*                   m_pWindow = nullptr;

    GameState*                    m_pCurrentState = nullptr;
    vector<unique_ptr<GameState>> m_AllStates;
    vector<shared_ptr<Sound>>     m_LoadedSounds;
    mutable TexturesVec           m_LoadedTextures;
};