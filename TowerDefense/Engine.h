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
    void DisplayTexture(const string& FileName, int x, int y, int w, int h);
    void DestroyTextures();
    vec2i GetTextureSize(const string& FileName);
    vec2 GetMousePos()const;

private:
    bool                          m_IsRunning = true;
                                  
    static Engine*                pSingleton;
    SDL_Renderer*                 m_pRenderer = nullptr;
    SDL_Window*                   m_pWindow = nullptr;

    GameState*                    m_pCurrentState = nullptr;
    vector<unique_ptr<GameState>> m_AllStates;
    vector<shared_ptr<Sound>>     m_LoadedSounds;
    vector<shared_ptr<Texture>>   m_LoadedTextures;
};