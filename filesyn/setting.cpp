#include "setting.h"

setting::setting( )
{
}

setting::setting(const setting&)
{
}

setting& setting::operator=(const setting&)
{
    // TODO: �ڴ˴����� return ���
}

setting& setting::GetSetting( )
{
    static setting instance;
    
    return instance;
}

setting::~setting( )
{
}