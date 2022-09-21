#include "Definition.h"

int Definition::GetIntValue(const string& ValueName, int DefaultValue) const
{
    return m_Data.child("Data").child(ValueName.c_str()).text().as_int(DefaultValue);
}

float Definition::GetFloatValue(const string& ValueName, float DefaultValue) const
{
    return m_Data.child("Data").child(ValueName.c_str()).text().as_float(DefaultValue);
}

string Definition::GetStringValue(const string& ValueName, string DefaultValue) const
{
    return m_Data.child("Data").child(ValueName.c_str()).text().as_string(DefaultValue.c_str());
}

vector<string> Definition::GetStringValuesVector(const string& ValueName)const
{
     vector<string> vec;

     auto dataNode = m_Data.child("Data");
     auto frameNode = dataNode.child(ValueName.c_str());

     if (dataNode && frameNode)
     {
         vec.push_back(frameNode.text().as_string());

         while (frameNode = frameNode.next_sibling(ValueName.c_str()))
         {
             vec.push_back(frameNode.text().as_string());
         }
     }

     return vec;
}

float Definition::GetAttributeFloatValue(const string& AttributeName, float DefaultValue) const
{
    return m_Data.child("Data").attribute(AttributeName.c_str()).as_float(DefaultValue);
}

bool Definition::LoadFromFile(const string& FileName)
{
    if(!m_Data.load_file(("../Data/" + FileName).c_str()))
        return false;

    m_Name = FileName;

    return true;
}

string Definition::GetName() const
{
    return m_Name;
}
