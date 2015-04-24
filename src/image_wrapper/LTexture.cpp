#include <imageWrapper.h>

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>



LTexture::LTexture()
{
	//Initialize
	_texture = NULL;
	_width = 0;
	_height = 0;
	_pixels = NULL;
	_pitch = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to display format
		SDL_Surface* formattedSurface = SDL_ConvertSurface( loadedSurface, SDL_GetWindowSurface( gWindow )->format, NULL );
		if( formattedSurface == NULL )
		{
			printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture( gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
			if( newTexture == NULL )
			{
				printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				//Lock texture for manipulation
				SDL_LockTexture( newTexture, NULL, &_pixels, &_pitch );

				//Copy loaded/formatted surface pixels
				memcpy( _pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );

				//Unlock texture to update
				SDL_UnlockTexture( newTexture );
				_pixels = NULL;

				//Get image dimensions
				_width = formattedSurface->w;
				_height = formattedSurface->h;
			}

			//Get rid of old formatted surface
			SDL_FreeSurface( formattedSurface );
		}	
		
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	_texture = newTexture;
	return _texture != NULL;
}

bool LTexture::createForWindow() {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* formattedSurface = SDL_ConvertSurface( SDL_GetWindowSurface( gWindow ), SDL_GetWindowSurface( gWindow )->format, NULL );
	newTexture = SDL_CreateTexture( gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
	if( newTexture == NULL ) {
		printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
	}else{
		//Lock texture for manipulation
		SDL_LockTexture( newTexture, NULL, &_pixels, &_pitch );

		//Unlock texture to update
		SDL_UnlockTexture( newTexture );
		_pixels = NULL;

		//Get image dimensions
		_width = formattedSurface->w;
		_height = formattedSurface->h;
	}
	//Get rid of old formatted surface
	SDL_FreeSurface( formattedSurface );
	//Return success
	_texture = newTexture;
	return _texture != NULL;
}

bool LTexture::createForPixelData(int w, int h) {
	SDL_Texture* newTexture = NULL;
	
	newTexture = SDL_CreateTexture( gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, w, h );
	if( newTexture == NULL ) {
		printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
	}else{
		//Lock texture for manipulation
		SDL_LockTexture( newTexture, NULL, &_pixels, &_pitch );

		//Unlock texture to update
		SDL_UnlockTexture( newTexture );
		_pixels = NULL;

		//Get image dimensions
		_width = w;
		_height = h;
	}
	//Return success
	_texture = newTexture;
	return _texture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( _texture != NULL )
	{
		SDL_DestroyTexture( _texture );
		_texture = NULL;
		_width = 0;
		_height = 0;
		_pixels = NULL;
		_pitch = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( _texture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( _texture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( _texture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, _width, _height };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, _texture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return _width;
}

int LTexture::getHeight()
{
	return _height;
}

bool LTexture::lockTexture()
{
	bool success = true;

	//Texture is already locked
	if( _pixels != NULL )
	{
		printf( "Texture is already locked!\n" );
		success = false;
	}
	//Lock texture
	else
	{
		if( SDL_LockTexture( _texture, NULL, &_pixels, &_pitch ) != 0 )
		{
			printf( "Unable to lock texture! %s\n", SDL_GetError() );
			success = false;
		}
	}

	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if( _pixels == NULL )
	{
		printf( "Texture is not locked!\n" );
		success = false;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture( _texture );
		_pixels = NULL;
		_pitch = 0;
	}

	return success;
}

void* LTexture::getPixels()
{
	return _pixels;
}

int LTexture::getPitch()
{
	return _pitch;
}

LTexture* gTexture = new LTexture;
