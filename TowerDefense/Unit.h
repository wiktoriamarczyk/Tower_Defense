#pragma once
#include "GameObject.h"
#include "Definition.h"

class Animator
{
public:
    void AddTexture(eTextureType Type, const string& FileName);
    void DisplayTexture(sf::RenderWindow& Renderer, vec2 Position, DisplayParameters Parameters);
    bool StartAnimation(eTextureType Type, bool Loop, function<void()> FinishCallback={});
    void Update(float DeltaTime);
    void SetFrameSpeed(float FrameSpeed);
private:
    struct TextureData
    {
        eTextureType m_Type;
        string m_FileName;
        uint32_t m_FramesCount;
    };
    vector<TextureData>                 m_TextureNames;
    TextureData                         m_CurrentTexture;
    bool                                m_Loop = false;
    bool                                m_Working = false;
    function<void()>                    m_FinishCallback;
    float                               m_CurrentFrame = 0; //< numer klatki, ktory w display zostanie przyciety do inta
    float                               m_FrameSpeed = 12.f; //< ile klatek animacji zostanie wyswietlonych w ciagu 1s
};

class Unit : public GameObject
{
public:
    Unit(vec2 Position);
    void Update(float DeltaTime)override;
    void Render(sf::RenderWindow& Renderer)override;
    bool OnMouseButtonDown(int Button)override;
    bool IsCursorOverObject()const override;
    bool FillToolTip(ToolTip& MyToolTip)const override;
    void MoveTo(vector<vec2> TargetPosition);
    void Initialize(const Definition& Def);
    void OnHit(Damage DamageValue);
    void ChangeTexture(eTextureType Texture);

    bool GetDamageStatus()const;
    int GetHP()const;
    int GetMaxHP()const;
    float GetMoneyForKill()const;

    void SetDamageStatus(bool Status);
    void SetHP(int Value);

private:
    //vector<pair<eTextureType, string>> m_TextureNames;
    //pair<eTextureType, string>         m_CurrentTexture;
    Animator     m_Animator;
    vector<vec2> m_TargetPositions;
    float        m_Speed = 10.f;
    float        m_HP = 100.f;
    float        m_MaxHP = 100.f;
    float        m_MoneyForKill = 0;
    bool         m_IsHurt = false;
    Resistances  m_Resistances;
};