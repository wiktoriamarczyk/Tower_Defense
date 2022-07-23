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