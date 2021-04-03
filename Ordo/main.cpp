#include "SDL.h"
#include <glad/glad.h> 
#include <stdio.h>
#include <iostream>

#include "SDLManager.h"
#include "Window.h"
#include "Engine.h"
#include "Files.h"

int main(int argc, char* args[])
{
    Window window = Window("Ordo", 800, 600);

    SDLManager sdlManager = SDLManager(window);    

    Engine engine(window);

    sdlManager.quit();

    return 0;
}