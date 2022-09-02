#pragma once
#include "Lib/pugixml.hpp"
#include "Common.h"

class Definition
{
public:
    int GetIntValue(const string& ValueName, int DefaultValue=0)const;
    float GetFloatValue(const string& ValueName, float DefaultValue=0)const;
    string GetStringValue(const string& ValueName, string DefaultValue="")const;
    vector<string> GetStringValuesVector(const string& ValueName="")const;
    bool LoadFromFile(const string& FileName);
    string GetName()const;
private:
    string             m_Name;
    pugi::xml_document m_Data;
};