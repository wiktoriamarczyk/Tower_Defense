#include "Common.h"

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

string ToString(float value, int Precision)
{
    char Buffer[64];
    sprintf(Buffer, "%.*f" , Precision , value);
    return Buffer;
}

int GetRandInt(int Min, int Max)
{
    if (Min == Max)
    {
        return Max;
    }
    return rand() % (Max - Min + 1) + Min;
}

float GetRandFloat(float Min, float Max)
{
    float FloatDiff = Max - Min;
    if (FloatDiff <= 1)
        return Min + (FloatDiff)*GetNormalizedFloat();

    const int IntDiff = int(FloatDiff);
    return Min + GetRandInt(0, IntDiff-1) + (1+FloatDiff-IntDiff)* GetNormalizedFloat();
}

float GetNormalizedFloat()
{
    return rand() / float(RAND_MAX);
}