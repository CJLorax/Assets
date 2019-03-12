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

	#include <unistd.h>
#endif

#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_net.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_ttf.h>

	#include <unistd.h>
#endif

#if defined(_WIN32) || (_WIN64)
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_net.h>
	#include <SDL_mixer.h>
	#include <SDL_ttf.h>

	#include <direct.h>
	#define getcwd _getcwd
#endif


#include <stdio.h>
#include <iostream>
using namespace std;

// variables for the background movement and position
int bkgdSpeed = 100;

// create rectangles for the background 1 and 2 textures
SDL_Rect bkgd1Pos, bkgd2Pos;

// background float vars for movement
float b1posX =  0, b1posY = 0;
float b2posX =  0, b2posY = -768;

// deltaTime init() - for frame rate ind.
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

// create rectangles for the menu graphics
SDL_Rect titlePos, instructPos, player1ButtonPos, player2ButtonPos, quitPos;


void updateBackground(float deltaTime)
{
	// update bkgd1's float position
	b1posY += (bkgdSpeed * 1) * deltaTime;
	bkgd1Pos.y = (int)(b1posY + 0.5f);

	// check to see if off bottom of the screen
	if(bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y = -768;
		b1posY = bkgd1Pos.y;
	}

	// update bkgd2's float position
	b2posY += (bkgdSpeed * 1) * deltaTime;
	bkgd2Pos.y = (int)(b2posY + 0.5f);

	// check to see if off bottom of the screen
	if(bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y = -768;
		b2posY = bkgd2Pos.y;
	}
}



// analog joystick deadzone
const int JOYSTICK_DEAD_ZONE = 8000;

// joysticks direction vars
float xDir, yDir;

// cursor floats for movement
float pos_X, pos_Y;

SDL_Rect cursorPos, activePos;

int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event)
{

	if( event.axis == 0)
	{
		if(event.value < -JOYSTICK_DEAD_ZONE)
		{
			xDir = -1.0f;

		}else if(event.value > JOYSTICK_DEAD_ZONE)
		{
			xDir = 1.0f;

		}else
		{
			xDir = 0.0f;
		}
	}

	if( event.axis == 1)
	{
		if(event.value < -JOYSTICK_DEAD_ZONE)
		{
			yDir = -1.0f;

		}else if(event.value > JOYSTICK_DEAD_ZONE)
		{
			yDir = 1.0f;

		}else
		{
			yDir = 0.0f;
		}
	}
}

void updateCursor(float deltaTime)
{
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_Y + 0.5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if(cursorPos.x < 0){
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if(cursorPos.x > 1024 - cursorPos.w){
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	if(cursorPos.y < 0){
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.h){
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}

// vars for cursor rollover
bool players1Over = false, players2Over = false, instructionsOver = false, quitOver = false, menuOver = false;

#include "player.h"

int main(int argc, char* argv[]) {


#if defined(__APPLE__)
	cout << "Running on OS X..." << endl;

	// get current working directory and append the images folder to it
	string s_cwd(getcwd(NULL,0));
	string s_cwd_images = s_cwd + "/Assets/images/";

	cout << s_cwd_images << endl;
#endif

#if defined(__linux__)
	cout << "Running on Ubuntu Linux..." << endl;

	// get current working directory and append the images folder to it
	string s_cwd(getcwd(NULL,0));
	string s_cwd_images = s_cwd + "/Assets/images/";

	cout << s_cwd_images << endl;
#endif

#if defined(_WIN32) || (_WIN64)
	cout << "Running on Windows 10..." << endl;

	// get current working directory and append the images folder to it
	string s_cwd(getcwd(NULL, 0));
	string s_cwd_images = s_cwd + "\\Assets\\images\\";

#endif

    SDL_Window *window;                    // Declare a pointer

    // create a renderer variable
    SDL_Renderer *renderer = NULL;

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

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player player1 = Player(renderer, 0, s_cwd_images.c_str(), 250.0, 500.0);


// ********** red proto graphic *************
    // create a path to the background image
    string BKGDpath = s_cwd_images + "placeholder.png";

    // create a SDL surface
    SDL_Surface *surface = IMG_Load(BKGDpath.c_str());

    // create texture
    SDL_Texture *proto;

    // place surface into the texture
    proto = SDL_CreateTextureFromSurface(renderer, surface);

    // create the rectangle for the texture
    SDL_Rect protoPos;

    protoPos.x = 0;
    protoPos.y = 0;
    protoPos.w = 256;
    protoPos.h = 256;

	// free the surface
	SDL_FreeSurface(surface);

// ********** red proto graphic *************

	// ********** Menu Graphics *************

	// background images
	string bkgdPath = s_cwd_images + "bkgd.png";

	// create a SDL surface
	surface = IMG_Load(bkgdPath.c_str());

	// create bkgd texture
	SDL_Texture *bkgd1;

	// place surface into the texture
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	// create bkgd texture
	SDL_Texture *bkgd2;

	// place surface into the texture
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

    bkgd1Pos.x = 0;
    bkgd1Pos.y = 0;
    bkgd1Pos.w = 1024;
    bkgd1Pos.h = 768;

    bkgd2Pos.x = 0;
    bkgd2Pos.y = -768;
    bkgd2Pos.w = 1024;
    bkgd2Pos.h = 768;


    // title image
    string imagePath = s_cwd_images + "mainTitle.png";

    // create a SDL surface
    surface = IMG_Load(imagePath.c_str());

    // create bkgd texture
    SDL_Texture *title;

    	// place surface into the texture
    title = SDL_CreateTextureFromSurface(renderer, surface);

    // free the surface
    SDL_FreeSurface(surface);


    titlePos.y = 40;
    titlePos.w = 487;
    titlePos.h = 93;
    titlePos.x = 1024/2 - titlePos.w/2;

    // instructions image
    imagePath = s_cwd_images + "instructN.png";

    // create a SDL surface
    surface = IMG_Load(imagePath.c_str());

    // create instructions texture
    SDL_Texture *instruct;

    // place surface into the texture
    instruct = SDL_CreateTextureFromSurface(renderer, surface);

    // free the surface
    SDL_FreeSurface(surface);

	// player 1 over image
	imagePath = s_cwd_images + "instructO.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *instructO;

	// place surface into the texture
	instructO = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);


    instructPos.y = 220;
    instructPos.w = 362;
    instructPos.h = 62;
    instructPos.x = 1024/2 - instructPos.w/2;

    // player 1 image
    imagePath = s_cwd_images + "oneN.png";

    // create a SDL surface
    surface = IMG_Load(imagePath.c_str());

    // create player 1 texture
    SDL_Texture *player1Button;

    // place surface into the texture
    player1Button = SDL_CreateTextureFromSurface(renderer, surface);

    // free the surface
    SDL_FreeSurface(surface);

    // player 1 over image
    imagePath = s_cwd_images + "oneO.png";

    // create a SDL surface
    surface = IMG_Load(imagePath.c_str());

    // create player 1 over texture
    SDL_Texture *player1OButton;

    // place surface into the texture
    player1OButton = SDL_CreateTextureFromSurface(renderer, surface);

    // free the surface
    SDL_FreeSurface(surface);


    player1ButtonPos.y = 330;
    player1ButtonPos.w = 494;
    player1ButtonPos.h = 69;
    player1ButtonPos.x = 1024/2 - player1ButtonPos.w/2;

    // player 2 image
    imagePath = s_cwd_images + "twoN.png";

    // create a SDL surface
    surface = IMG_Load(imagePath.c_str());

    // create player 2 texture
    SDL_Texture *player2Button;

    // place surface into the texture
    player2Button = SDL_CreateTextureFromSurface(renderer, surface);

    // free the surface
    SDL_FreeSurface(surface);

	// player 1 over image
	imagePath = s_cwd_images + "twoO.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *player2OButton;

	// place surface into the texture
	player2OButton = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);


    player2ButtonPos.y = 445;
    player2ButtonPos.w = 502;
    player2ButtonPos.h = 69;
    player2ButtonPos.x = 1024/2 - player2ButtonPos.w/2;

    // quit image
    imagePath = s_cwd_images + "quitN.png";

    // create a SDL surface
    surface = IMG_Load(imagePath.c_str());

    // create quit texture
    SDL_Texture *quitN;

    // place surface into the texture
    quitN = SDL_CreateTextureFromSurface(renderer, surface);

    // free the surface
    SDL_FreeSurface(surface);

	// quit over image
	imagePath = s_cwd_images + "quitO.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *quitO;

	// place surface into the texture
	quitO = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);


    quitPos.y = 555;
    quitPos.w = 299;
    quitPos.h = 69;
    quitPos.x = 1024/2 - quitPos.w/2;

    // quit over image
	imagePath = s_cwd_images + "cursor.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *cursor;

	// place surface into the texture
	cursor = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);



	cursorPos.x = 0;
	cursorPos.y = 0;
	cursorPos.w = 71;
	cursorPos.h = 74;

	activePos.x = 10;
	activePos.y = 10;
	activePos.w = 10;
	activePos.h = 10;


	// quit over image
	imagePath = s_cwd_images + "instructText.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *instructText;

	// place surface into the texture
	instructText = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	SDL_Rect instructTextPos;


	instructTextPos.y = 40;
	instructTextPos.w = 502;
	instructTextPos.h = 377;
	instructTextPos.x = 1024/2 - instructTextPos.w/2;


	// quit over image
	imagePath = s_cwd_images + "menuSmallN.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *menuSmallN;

	// place surface into the texture
	menuSmallN = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	SDL_Rect menuPos;

	menuPos.x = 793;
	menuPos.y = 670;
	menuPos.w = 214;
	menuPos.h = 42;

	// quit over image
	imagePath = s_cwd_images + "menuSmallO.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *menuSmallO;

	// place surface into the texture
	menuSmallO = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	// quit over image
	imagePath = s_cwd_images + "winText.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *winText;

	// place surface into the texture
	winText = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	SDL_Rect winTextPos;

	winTextPos.y = 40;
	winTextPos.w = 493;
	winTextPos.h = 378;
	winTextPos.x = 1024/2 - winTextPos.w/2;

	// quit over image
	imagePath = s_cwd_images + "loseText.png";

	// create a SDL surface
	surface = IMG_Load(imagePath.c_str());

	// create player 1 over texture
	SDL_Texture *loseText;

	// place surface into the texture
	loseText = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	SDL_Rect loseTextPos;

	loseTextPos.y = 40;
	loseTextPos.w = 494;
	loseTextPos.h = 378;
	loseTextPos.x = 1024/2 - loseTextPos.w/2;

    // ********** Menu Graphics *************



	// set up a game controller variable
	SDL_GameController *gameController = NULL;

	// open the game controller
	gameController = SDL_GameControllerOpen(0);

	// turn on controller events
	SDL_GameControllerEventState(SDL_ENABLE);

	// SDL event to handle input
	SDL_Event event;

	// set up the enum to handle multiple states
	enum GameState {MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};

	// var enum to track where we are in game
	GameState gameState = MENU;

	// boolean vars for the individual states
	bool menu, instructions, players1, players2, win, lose, quit = false;

	// enter program loop

	// basic game loop
	while(!quit)
	{
		switch(gameState)
		{
			case MENU:

				menu = true;
/*				cout << "The Game State is MENU" << endl;
				cout << "Press the A Button for Instructions" << endl;
				cout << "Press the B Button for 1 Player Game" << endl;
				cout << "Press the X Button for 2 Player Game" << endl;
				cout << "Press the Y Button for Quit Game" << endl;
				cout << " " << endl;*/

				while(menu)
				{
					// create deltaTime
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input
					if (SDL_PollEvent(&event)) {

						if (event.type == SDL_QUIT) {
							quit = true;
							menu = false;
							break;
						}

						// test for controller input
						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								// checks to see if this is controller 0
								if(event.cdevice.which == 0)
								{
									// if A button
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{

										if(players1Over){
											menu = false;
											gameState = PLAYERS1;
											players1Over = false;
										}
										if(players2Over){
											menu = false;
											gameState = PLAYERS2;
											players2Over = false;
										}
										if(instructionsOver){
											menu = false;
											gameState = INSTRUCTIONS;
											instructionsOver = false;
										}
										if(quitOver){
											menu = false;
											quit = true;
											quitOver = false;
										}
									}
								}
								break;

							case SDL_CONTROLLERAXISMOTION:

								moveCursor(event.caxis);

								break;



						} // end switch for menu event.type
					} // end poll event

					// Update Section
					updateBackground(deltaTime);

					updateCursor(deltaTime);

					//bool players1Over = false, players2Over = false, instructions2Over = false, quitOver = false, menuOver = false;

					// check for collision between cursor and buttons
					players1Over = SDL_HasIntersection(&activePos, &player1ButtonPos);
					players2Over = SDL_HasIntersection(&activePos, &player2ButtonPos);
					instructionsOver = SDL_HasIntersection(&activePos, &instructPos);
					quitOver = SDL_HasIntersection(&activePos, &quitPos);

					// Draw Section
					//clear the old buffer
					SDL_RenderClear(renderer);

					//prepare bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//prepare bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//prepare title
					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					if(instructionsOver)
					{
						//prepare instructions
						SDL_RenderCopy(renderer, instructO, NULL, &instructPos);
					}else
					{
						//prepare instructions
						SDL_RenderCopy(renderer, instruct, NULL, &instructPos);
					}

					if(players1Over)
					{
						//prepare 1 player button
						SDL_RenderCopy(renderer, player1OButton, NULL, &player1ButtonPos);
					}else
					{
						//prepare 1 player button
						SDL_RenderCopy(renderer, player1Button, NULL, &player1ButtonPos);
					}

					if(players2Over)
					{
						//prepare 2 player button
						SDL_RenderCopy(renderer, player2OButton, NULL, &player2ButtonPos);
					}else
					{
						//prepare 2 player button
						SDL_RenderCopy(renderer, player2Button, NULL, &player2ButtonPos);
					}

					if(quitOver)
					{
						//prepare quit button
						SDL_RenderCopy(renderer, quitO, NULL, &quitPos);
					}else
					{
						//prepare quit button
						SDL_RenderCopy(renderer, quitN, NULL, &quitPos);
					}

					//prepare cursor
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					// draw new info to the screen
					SDL_RenderPresent(renderer);

				} // end "menu" while
				break;

			//INSTRUCTIONS SCREEN
			case INSTRUCTIONS:

				instructions = true;
/*				cout << "The Game State is INSTRUCTIONS" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << " " << endl;*/

				while(instructions)
				{

					// create deltaTime
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input
					if (SDL_PollEvent(&event)) {

						if (event.type == SDL_QUIT) {
							quit = true;
							instructions = false;
							break;
						}

						// test for controller input
						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								// checks to see if this is controller 0
								if(event.cdevice.which == 0)
								{
									// if A button - Main Menu
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										instructions = false;
										gameState = MENU;
										menuOver = false;
										//cout << "TEST A" << endl;

									}
								}

								break;

							case SDL_CONTROLLERAXISMOTION:

								moveCursor(event.caxis);

								break;
						} // end switch for instructions event.type
					} // end poll event

					// Update Section
					updateBackground(deltaTime);

					updateCursor(deltaTime);

					// check for collision between cursor and buttons
					menuOver = SDL_HasIntersection(&activePos, &menuPos);


					// Draw Section
					//clear the old buffer
					SDL_RenderClear(renderer);

					//prepare bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//prepare bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//prepare title
					SDL_RenderCopy(renderer, instructText, NULL, &instructTextPos);

					if(menuOver)
					{
						//prepare quit button
						SDL_RenderCopy(renderer, menuSmallO, NULL, &menuPos);
					}else
					{
						//prepare quit button
						SDL_RenderCopy(renderer, menuSmallN, NULL, &menuPos);
					}

					//prepare cursor
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					// draw new info to the screen
					SDL_RenderPresent(renderer);


				} // end "instructions" while
				break;

			// PLAYERS 1
			case PLAYERS1:

				players1 = true;
/*				cout << "The Game State is PLAYERS1" << endl;
				cout << "Press the A Button for WIN screen" << endl;
				cout << "Press the B Button for LOSE screen" << endl;
				cout << " " << endl;*/

				while(players1)
				{

					// create deltaTime
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input
					if (SDL_PollEvent(&event)) {

						if (event.type == SDL_QUIT) {
							quit = true;
							players1 = false;
							break;
						}

						// test for controller input
						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								// checks to see if this is controller 0
								if(event.cdevice.which == 0)
								{
									// if A button - WIN
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
									{
										players1 = false;
										gameState = WIN;
										//cout << "TEST A" << endl;

									}

									// if B button - LOSE
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
									{
										players1 = false;
										gameState = LOSE;
										//cout << "TEST B" << endl;

									}

									player1.OnControllerButton(event.cbutton);
								}

								break;

							case SDL_CONTROLLERAXISMOTION:

									player1.OnControllerAxis(event.caxis);

									break;
						} // end switch for players1 event.type
					} // end poll event


					// Update Section
					updateBackground(deltaTime);

					player1.Update(deltaTime);


					// Draw Section
					//clear the old buffer
					SDL_RenderClear(renderer);

					//prepare bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//prepare bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					player1.Draw(renderer);

					//prepare title
					SDL_RenderCopy(renderer, player1Button, NULL, &player1ButtonPos);

					// draw new info to the screen
					SDL_RenderPresent(renderer);

				} // end "players1" while
				break;



				// PLAYERS 2
				case PLAYERS2:

					players2 = true;
//					cout << "The Game State is PLAYERS2" << endl;
//					cout << "Press the A Button for WIN screen" << endl;
//					cout << "Press the B Button for LOSE screen" << endl;
//					cout << " " << endl;

					while(players2)
					{
						// create deltaTime
						thisTime = SDL_GetTicks();
						deltaTime = (float)(thisTime - lastTime)/1000;
						lastTime = thisTime;

						// check for input
						if (SDL_PollEvent(&event)) {

							if (event.type == SDL_QUIT) {
								quit = true;
								players2 = false;
								break;
							}

							// test for controller input
							switch(event.type)
							{
								case SDL_CONTROLLERBUTTONDOWN:
									// checks to see if this is controller 0
									if(event.cdevice.which == 0)
									{
										// if A button - WIN
										if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
										{
											players2 = false;
											gameState = WIN;
											//cout << "TEST A" << endl;

										}

										// if B button - LOSE
										if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
										{
											players2 = false;
											gameState = LOSE;
											//cout << "TEST B" << endl;

										}
									}

									break;
							} // end switch for players2 event.type
						} // end poll event


						// Update Section
						updateBackground(deltaTime);


						// Draw Section
						//clear the old buffer
						SDL_RenderClear(renderer);

						//prepare bkgd1
						SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

						//prepare bkgd2
						SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

						//prepare title
						SDL_RenderCopy(renderer, player2Button, NULL, &player2ButtonPos);

						// draw new info to the screen
						SDL_RenderPresent(renderer);


					} // end "players2" while
					break;


					// WIN SCREEN
					case WIN:

						win = true;
/*						cout << "The Game State is WIN" << endl;
						cout << "Press the A Button for Main Menu" << endl;
						cout << " " << endl;*/

						while(win)
						{

							// create deltaTime
							thisTime = SDL_GetTicks();
							deltaTime = (float)(thisTime - lastTime)/1000;
							lastTime = thisTime;

							// check for input
							if (SDL_PollEvent(&event)) {

								if (event.type == SDL_QUIT) {
									quit = true;
									win = false;
									break;
								}

								// test for controller input
								switch(event.type)
								{
									case SDL_CONTROLLERBUTTONDOWN:
										// checks to see if this is controller 0
										if(event.cdevice.which == 0)
										{
											// if A button - Main Menu
											if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
											{
												win = false;
												gameState = MENU;
												menuOver = false;
												//cout << "TEST A" << endl;

											}
										}

										break;

									case SDL_CONTROLLERAXISMOTION:

									moveCursor(event.caxis);

									break;
								} // end switch for win event.type
							} // end poll event

							// Update Section
							updateBackground(deltaTime);

							updateCursor(deltaTime);

							// check for collision between cursor and buttons
							menuOver = SDL_HasIntersection(&activePos, &menuPos);


							// Draw Section
							//clear the old buffer
							SDL_RenderClear(renderer);

							//prepare bkgd1
							SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

							//prepare bkgd2
							SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

							//prepare title
							SDL_RenderCopy(renderer, winText, NULL, &winTextPos);

							if(menuOver)
							{
								//prepare quit button
								SDL_RenderCopy(renderer, menuSmallO, NULL, &menuPos);
							}else
							{
								//prepare quit button
								SDL_RenderCopy(renderer, menuSmallN, NULL, &menuPos);
							}

							//prepare cursor
							SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

							// draw new info to the screen
							SDL_RenderPresent(renderer);


						} // end "win" while
						break;


						// LOSE SCREEN
						case LOSE:

							lose = true;
/*							cout << "The Game State is LOSE" << endl;
							cout << "Press the A Button for Main Menu" << endl;
							cout << " " << endl;*/

							while(lose)
							{

								// create deltaTime
								thisTime = SDL_GetTicks();
								deltaTime = (float)(thisTime - lastTime)/1000;
								lastTime = thisTime;

								// check for input
								if (SDL_PollEvent(&event)) {

									if (event.type == SDL_QUIT) {
										quit = true;
										lose = false;
										break;
									}

									// test for controller input
									switch(event.type)
									{
										case SDL_CONTROLLERBUTTONDOWN:
											// checks to see if this is controller 0
											if(event.cdevice.which == 0)
											{
												// if A button - Main Menu
												if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
												{
													lose = false;
													gameState = MENU;
													menuOver = false;
													//cout << "TEST A" << endl;

												}
											}

											break;

										case SDL_CONTROLLERAXISMOTION:

										moveCursor(event.caxis);

										break;
									} // end switch for win event.type
								} // end poll event

								// Update Section
								updateBackground(deltaTime);

								updateCursor(deltaTime);

								// check for collision between cursor and buttons
								menuOver = SDL_HasIntersection(&activePos, &menuPos);

								// Draw Section
								//clear the old buffer
								SDL_RenderClear(renderer);

								//prepare bkgd1
								SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

								//prepare bkgd2
								SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

								//prepare title
								SDL_RenderCopy(renderer, loseText, NULL, &loseTextPos);

								if(menuOver)
								{
									//prepare quit button
									SDL_RenderCopy(renderer, menuSmallO, NULL, &menuPos);
								}else
								{
									//prepare quit button
									SDL_RenderCopy(renderer, menuSmallN, NULL, &menuPos);
								}

								//prepare cursor
								SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

								// draw new info to the screen
								SDL_RenderPresent(renderer);

							} // end "win" while
							break;


		} // end outer most switch
	} // end outer most while


    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}






