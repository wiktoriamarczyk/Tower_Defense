#include "Common.h"
#include "Engine.h"

#ifdef main
// erase SDL2 #define of 'main' keyword
#undef main
#endif

int main()
{
    Engine MyEngine;

    if (MyEngine.Initialize() == false)
    {
        return 1;
    }

    MyEngine.Loop();

    return 0;
}