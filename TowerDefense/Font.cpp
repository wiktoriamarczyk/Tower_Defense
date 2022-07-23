#include "Font.h"

void Font::LoadFont(const string& FileName)
{
    m_Dictionary = FillFontVector(FileName);
}

const CharacterData* Font::FindCharacter(char Character) const
{
    for (int i = 0; i < m_Dictionary.size(); ++i)
    {
        if (m_Dictionary[i].m_Character == Character)
            return &m_Dictionary[i];
    }
    return nullptr;
}

void Font::DrawText(sf::RenderWindow& Renderer, int PixelSize, int PosX, int PosY, const char* Text, sf::Color DrawColor)
{
    const int CharacterSpacing = PixelSize * 8;

    for (int i = 0; Text[i] != 0; ++i)
    {
        const CharacterData* pData = FindCharacter(Text[i]);
        if (pData == nullptr)
            continue;
        DrawLines(Renderer, PixelSize, PosX + i * CharacterSpacing, PosY, pData->m_ImageData, DrawColor);
    }
}
//==========================================================================================================
bool ReadLine(fstream& FileStream, string& OutString, int ExpectedSize)
{
    getline(FileStream, OutString);
    if (OutString.size() == ExpectedSize)
        return true;
    return false;
}

bool LoadSingleCharacter(fstream& FileStream, CharacterData& DataOut)
{
    // 1) kazdy symbol rozpoczyna sie znakiem reprezentujacym symbol
    // 2) nastepnie w 7 liniach maksymalnie po 7 znakow 'X' wystepuje graficzna reprezentacja symbolu
    // 3) ostatnia linia nie zawiera zadnych znakow, stanowi przerwe pomiedzy kolejnym symbolem czcionki

    // 1)
    string temp_char;
    if (!(ReadLine(FileStream, temp_char, 1)))
        return false;
    DataOut.m_Character = temp_char[0];
    // 2)
    string temp_line;
    for (int i = 0; i < 7; ++i)
    {
        if (!(ReadLine(FileStream, temp_line, 7)))
            return false;
        DataOut.m_ImageData.push_back(temp_line);
    }
    // 3)
    if (!(ReadLine(FileStream, temp_line, 0)))
        return false;

    return true;
}

vector<CharacterData> FillFontVector(const string& FileName)
{
    vector<CharacterData> Dictionary;

    fstream file(FileName.c_str());

    if (!file)
    {
        return Dictionary;
    }

    while (true)
    {
        if (file.eof())
        {
            return Dictionary;
        }

        CharacterData Data;

        // sprobuj wczytac dane o jednym symbolu do zmiennej Data, jesli sie nie udalo, zwroc pusty wektor
        if (LoadSingleCharacter(file, Data) == false)
        {
            return Dictionary;
        }
        // dodaj Data (dane o jednym symbolu) do wektora Dictionary
        Dictionary.push_back(Data);
    }
    // zwroc slownik odczytanych symboli
    return Dictionary;
}
//==========================================================================================================
void DrawLine(sf::RenderWindow& Renderer, int PixelSize, int PosX, int PosY, const string& Pixels, sf::Color DrawColor)
{
    for (int i = 0; i < Pixels.size(); ++i)
    {
        if (Pixels[i] != ' ')
        {
            sf::RectangleShape PixelRect(sf::Vector2f(PixelSize, PixelSize));
            PixelRect.setPosition(sf::Vector2f(PosX + i * PixelSize, PosY));
            PixelRect.setFillColor(DrawColor);
            Renderer.draw(PixelRect);
        }
    }
}

void DrawLines(sf::RenderWindow& Renderer, int PixelSize, int PosX, int PosY, const vector<string>& Lines, sf::Color DrawColor)
{
    for (int i = 0; i < Lines.size(); ++i)
    {
        DrawLine(Renderer, PixelSize, PosX, PosY + i * PixelSize, Lines[i], DrawColor);
    }
}
//==========================================================================================================
string ToString(int value)
{
    char buffer[16];
    _itoa_s(value, buffer, 10);

    return buffer;
}