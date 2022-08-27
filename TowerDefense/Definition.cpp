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
