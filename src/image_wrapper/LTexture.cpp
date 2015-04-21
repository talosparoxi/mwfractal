#include <imageWrapper.h>

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>



LTexture::LTexture() {
	//Initialize
	_Texture = NULL;
	_Width = 0;
	_Height = 0;
}

LTexture::~LTexture() {
	//destroy
	free();
}

bool LTexture::capture() {
	//empty space
	free();
	
	//the texture
	SDL_Texture* newTexture = NULL;
		
	//Create texture from surface pixels
	newTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight );
		
	//Return success
	_Texture = newTexture;
	return _Texture != NULL;
}

void LTexture::free() {
	if( _Texture != NULL ) {
		SDL_DestroyTexture( _Texture );
		_Texture = NULL;
		_Width = 0;
		_Height = 0;
	}
}

void LTexture::setColour( Uint8 red, Uint8 green, Uint8 blue ) {
	//Modulate texture
    SDL_SetTextureColorMod( _Texture, red, green, blue );
}
        

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ) {
	//set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, _Width, _Height };
	
	//Set clip rendering dimensions
	if( clip != NULL ) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	//Render to screen
	SDL_RenderCopyEx( gRenderer, _Texture, clip, &renderQuad, angle, center, flip );
}

LTexture* gTexture = new LTexture;
