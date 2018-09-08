#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <Dot.h>

Texture::Texture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 64;
	mHeight = 64;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

void Texture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	//Color key image
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

	//Get image dimensions
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
		
	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	mTexture = newTexture;
}

void Texture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(double x, double y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}

//---------------------------Dot Class-----------------------------\\

Dot::Dot()
{

	//Set collision box dimension to the size of the dot
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

}

void Dot::handleEvent(SDL_Event& e)
{
	//Jumping
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
	{
		mVelY -= DOT_VEL;
	}

	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_a: mVelX -= .5 * DOT_VEL; break;
		case SDLK_d: mVelX += .5 * DOT_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_a: mVelX += .5 * DOT_VEL; break;
		case SDLK_d: mVelX -= .5 * DOT_VEL; break;
		}
	}
}

void Dot::move(SDL_Rect& wall)
{

	//Gravity
	if (mVelY < DOT_VEL && (mPosY + DOT_HEIGHT < SCREEN_HEIGHT)) {
		mVelY += 2;
	}
	//Move the dot left or right by the value of VelX and moves the hitbox to the new position of the dot
	mPosX += mVelX;
	mCollider.x = mPosX;

	//If the dot collided or went too far to the left or right
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, wall))
	{
		//Move back
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}

	//Move the dot up or down by the value of VelY and moves the hitbox to the new position of the dot
	mPosY += mVelY;
	mCollider.y = mPosY;

	//If the dot collided or went too far up or down
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(mCollider, wall))
	{
		//Move back
		mPosY -= mVelY;
		mVelY = 0;
		mCollider.y = mPosY;
	}
}

bool Dot::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//rect a is the hitbox of the dot and rect b is the wall
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

void Dot::render()
{
	//Show the dot
	//texture.render(mPosX, mPosY);
}

double Dot::getPosX() {
	return mPosX;
}

double Dot::getPosY() {
	return mPosY;
}