// for cross platform dev
#if defined(__APPLE__)
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
	//#include <SDL2_net/SDL_net.h>
	//#include <SDL2_mixer/SDL_mixer.h>
	//#include <SDL2_ttf/SDL_ttf.h>

	#include <unistd.h>
#endif

#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	//#include <SDL2/SDL_net.h>
	//#include <SDL2/SDL_mixer.h>
	//#include <SDL2/SDL_ttf.h>

	#include <unistd.h>
#endif

#if defined(_WIN32) || (_WIN64)
	#include <SDL.h>
	#include <SDL_image.h>
	//#include <SDL_net.h>
	//#include <SDL_mixer.h>
	//#include <SDL_ttf.h>

	#include <direct.h>
	#define getcwd _getcwd
#endif


#include <string>
#include <iostream>
using namespace std;

class Bullet{

public:

	bool active;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir, yDir;

	float speed;

	float pos_X, pos_Y;

	Bullet(SDL_Renderer *renderer, string filePath, float x, float y);

	void Update(float deltaTime);

	void Draw(SDL_Renderer *renderer);

	~Bullet();

};
