#include "Common.h"

bool SDL_IsKeyPressed(SDL_Scancode Code)
{
    const Uint8* Keys = SDL_GetKeyboardState(NULL);
    return Keys[Code] != 0;
}
