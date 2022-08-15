#include "Common.h"
#include "Engine.h"

#if defined(WIN32)
    #include<Windows.h>
#endif

int main()
{
    Engine MyEngine;

    if (MyEngine.Initialize() == false)
    {
        return 1;
    }
    
#if defined(WIN32)
    ShowWindow( MyEngine.GetSingleton()->GetWindow().getSystemHandle() , SW_MAXIMIZE);
#endif

    MyEngine.Loop();

    return 0;
}