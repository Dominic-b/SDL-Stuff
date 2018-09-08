#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <Dot.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
 
//Starts up SDL and creates window
void init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

Texture texture;
Dot dot;

void init()
{

	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	//Create window
	gWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		
	//Create vsynced renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	(IMG_Init(imgFlags) & imgFlags);
}

void close()
{
	//Free loaded images
	texture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	init();
	texture.loadFromFile("Dot 2.png");
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//The dot that will be moving around on the screen
	Dot dot;

	//Set the wall
	SDL_Rect wall;
	wall.x = SCREEN_WIDTH / 3 * 2;
	wall.y = SCREEN_HEIGHT / 4 * 3;
	wall.w = SCREEN_WIDTH / 3;
	wall.h = 20;
	//screen width - 640, screen height - 480
	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//Handle input for the dot
			dot.handleEvent(e);
		}

		//Move the dot and check collision
		dot.move(wall);

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render wall
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &wall);

		//Render dot
		texture.render(dot.getPosX(), dot.getPosY());

		//Update screen
		SDL_RenderPresent(gRenderer);
		
		
	}
		
	//Free resources and close SDL
	close();
	return 0;
}