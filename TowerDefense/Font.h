#pragma once
#include "Common.h"

struct CharacterData
{
    vector<string> m_ImageData;
    char m_Character;
};


class Font
{
    vector<CharacterData> m_Dictionary;
public:
    void LoadFont(const string& FileName);
    const CharacterData* FindCharacter(char Character)const;
    void DrawText(SDL_Renderer* pRenderer, int PixelSize, int PosX, int PosY, const char* Text, Uint8 R=255, Uint8 G=255, Uint8 B=255);
};


bool ReadLine(fstream& FileStream, string& OutString, int ExpectedSize);
bool LoadSingleCharacter(fstream& FileStream, CharacterData& DataOut);
vector<CharacterData> FillFontVector(const string& FileName);
void DrawLine(SDL_Renderer* pRenderer, int PixelSize, int PosX, int PosY, const string& Pixels);
void DrawLines(SDL_Renderer* pRenderer, int PixelSize, int PosX, int PosY, const vector<string>& Lines, Uint8 R, Uint8 G, Uint8 B);
string ToString(int value);