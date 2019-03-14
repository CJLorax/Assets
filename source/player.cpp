#include "player.h"

// analog joystick deadzone
const int JOYSTICK_DEAD_ZONE = 8000;


Player::Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y)
{
	//set player num - 0 or 1
	playerNum = pNum;

	//set speed
	speed = 500.0f;

	if(playerNum == 0){
		playerPath = filePath + "player1.png";
	}else{
		playerPath = filePath + "player2.png";
	}

	surface = IMG_Load(playerPath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	posRect.x = x;
	posRect.y = y;

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	string bulletPath;

	if(playerNum == 0){
		bulletPath = filePath + "bullet1.png";
	}else{
		bulletPath = filePath + "bullet2.png";
	}

	cout << bulletPath << endl;



	for(int i = 0; i < 10; i++)
	{
		Bullet tempBullet(renderer, bulletPath, i + 5, i + 5);

		bulletList.push_back(tempBullet);
	}

}

void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	if(event.which == 0 && playerNum == 0)
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

	if(event.which == 1 && playerNum == 1)
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
}

void Player::CreateBullet()
{

	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active == false)
		{
			bulletList[i].active = true;

			bulletList[i].posRect.x = (pos_X + (posRect.w/2));

			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w/2));

			bulletList[i].posRect.y = posRect.y;

			bulletList[i].pos_X = pos_X;

			bulletList[i].pos_Y = pos_Y;

			break;
		}
	}

}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if(event.which == 0 && playerNum == 0)
	{
		if(event.button == 0)
		{
			cout << "player 1 Button A" << endl;

			CreateBullet();
		}
	}

	if(event.which == 1 && playerNum == 1)
	{
		if(event.button == 0)
		{
			cout << "player 2 Button A" << endl;

			CreateBullet();
		}
	}
}

void Player::Update(float deltaTime)
{
	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;

	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if(posRect.x < 0){
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w){
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y < 0){
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h){
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active == true)
		{
			bulletList[i].Update(deltaTime);
		}
	}

}

void Player::Draw(SDL_Renderer *renderer){

	SDL_RenderCopy(renderer, texture, NULL, &posRect);

	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active == true)
		{
			bulletList[i].Draw(renderer);
		}
	}


}

Player::~Player()
{
	SDL_DestroyTexture(texture);
}




