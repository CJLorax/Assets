//============================================================================
// Name        : SDLCrossPlatform.cpp
// Author      : Casey Jones
// Version     : 1.0
// Copyright   : @ 2019
// Description : SDL cross platform project in C++, Ansi-style
//============================================================================

#if defined(__APPLE__)
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
	#include <SDL2_net/SDL_net.h>
	#include <SDL2_mixer/SDL_mixer.h>
	#include <SDL2_ttf/SDL_ttf.h>
#endif

#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_net.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_ttf.h>
#endif

#if defined(_WIN32) || (_WIN64)
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_net.h>
	#include <SDL_mixer.h>
	#include <SDL_ttf.h>
#endif


#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {


#if defined(__APPLE__)
	cout << "Running on OS X..." << endl;
#endif

#if defined(__linux__)
	cout << "Running on Ubuntu Linux..." << endl;
#endif

#if defined(_WIN32) || (_WIN64)
	cout << "Running on Windows 10..." << endl;
#endif





    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1024,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }


    bool isquit = false;
    SDL_Event event;
    while (!isquit) {
        if (SDL_PollEvent( & event)) {
            if (event.type == SDL_QUIT) {
                isquit = true;
            }
        }
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}






