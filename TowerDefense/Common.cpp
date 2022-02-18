#include "Common.h"

bool SDL_IsKeyPressed(SDL_Scancode Code)
{
    const Uint8* Keys = SDL_GetKeyboardState(NULL);
    return Keys[Code] != 0;
}

vector<int> ConvertCharVecToInt(vector<char>& CharsToConvert)
{
    vector<int> ConvertedVec {};

    for (int i = 0; i < CharsToConvert.size(); ++i)
    {
        int tmp = CharsToConvert[i] - '0';
        ConvertedVec.push_back(tmp);
    }

    return ConvertedVec;
}