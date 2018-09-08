#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#pragma once
class Texture
{
public:

	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Loads image at specified path
	void loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(double x, double y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//---------------------------Dot Class-----------------------------\\

class Dot {
public:
	//Screen Sizey thingo
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	//The dimensions of the dot
	static const int DOT_WIDTH = 64;
	static const int DOT_HEIGHT = 64;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 30;

	//Initializes the variables
	Dot();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Box collision detector
	bool checkCollision(SDL_Rect a, SDL_Rect b);

	//Moves the dot and checks collision
	void move(SDL_Rect& wall);

	//Shows the dot on the screen
	void render();

	double getPosX();
	double getPosY();

private:
	//The X and Y offsets of the dot
	double mPosX = SCREEN_WIDTH / 2 - DOT_WIDTH / 2;
	double mPosY = SCREEN_HEIGHT / 4 - DOT_HEIGHT / 2;

	//The velocity of the dot
	double mVelX = 0;
	double mVelY = 0;

	//Dot's collision box
	SDL_Rect mCollider;
};