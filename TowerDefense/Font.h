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
    void DrawText(sf::RenderWindow& Renderer, int PixelSize, int PosX, int PosY, const char* Text, sf::Color DrawColor = sf::Color::White);
};


bool ReadLine(fstream& FileStream, string& OutString, int ExpectedSize);
bool LoadSingleCharacter(fstream& FileStream, CharacterData& DataOut);
vector<CharacterData> FillFontVector(const string& FileName);
void DrawLine(sf::RenderWindow& Renderer, int PixelSize, int PosX, int PosY, const string& Pixels, sf::Color DrawColor);
void DrawLines(sf::RenderWindow& Renderer, int PixelSize, int PosX, int PosY, const vector<string>& Lines, sf::Color DrawColor);