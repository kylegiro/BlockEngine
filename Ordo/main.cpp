#include "SDL.h"
#include <glad/glad.h> 
#include <stdio.h>

#include "SDLManager.h"
#include "Window.h"
#include "Engine.h"

int main(int argc, char* args[])
{
    SDLManager sdlManager = SDLManager();

    Window window = Window();

    Engine engine(window);

    sdlManager.quit();

    return 0;
}