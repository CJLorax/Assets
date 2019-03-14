// for cross platform dev
#if defined(__APPLE__)
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
//	#include <SDL2_net/SDL_net.h>
//	#include <SDL2_mixer/SDL_mixer.h>
//	#include <SDL2_ttf/SDL_ttf.h>

	#include <unistd.h>
#endif

#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
//	#include <SDL2/SDL_net.h>
//	#include <SDL2/SDL_mixer.h>
//	#include <SDL2/SDL_ttf.h>

	#include <unistd.h>
#endif

#if defined(_WIN32) || (_WIN64)
	#include <SDL.h>
	#include <SDL_image.h>
//	#include <SDL_net.h>
//	#include <SDL_mixer.h>
//	#include <SDL_ttf.h>

	#include <direct.h>
	#define getcwd _getcwd
#endif


#include <string>
#include <iostream>
using namespace std;

#include <vector>
#include "bullet.h"

class Player{

public:

	vector<Bullet> bulletList;

	string playerPath;

	int playerNum;

	SDL_Surface *surface;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir, yDir;

	float speed;

	float pos_X, pos_Y;

	Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y);

	void OnControllerAxis(const SDL_ControllerAxisEvent event);

	void OnControllerButton(const SDL_ControllerButtonEvent event);

	void Update(float deltaTime);

	void Draw(SDL_Renderer *renderer);

	~Player();

private:

	void CreateBullet();

};
