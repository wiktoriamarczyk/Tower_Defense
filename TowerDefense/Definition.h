#pragma once
#include "Lib/pugixml.hpp"
#include "Common.h"

class Definition
{
public:
    int GetIntValue(const string& ValueName, int DefaultValue)const;
    float GetFloatValue(const string& ValueName, float DefaultValue)const;
    string GetStringValue(const string& ValueName, string DefaultValue)const;
    vector<string> GetStringValuesVector(const string& ValueName)const;
    bool LoadFromFile(const string& FileName);
    string GetName()const;
private:
    string             m_Name;
    pugi::xml_document m_Data;
};