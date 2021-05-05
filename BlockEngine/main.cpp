#include "SDL.h"
#include <glad/glad.h> 
#include <stdio.h>
#include <iostream>

#include "SDLManager.h"
#include "Window.h"
#include "Engine.h"
#include "Files.h"
#include "Shader.h"

int main(int argc, char* args[])
{
    Window window = Window("BlockEngine", 800, 800);

    SDLManager sdlManager = SDLManager(window);        

    Engine engine(sdlManager.getContext(), window);

    sdlManager.quit();

    return 0;
}