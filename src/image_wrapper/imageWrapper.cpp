#include <imageWrapper.h>

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

class LTexture {
	public:
		//initializer
		LTexture();
		
		//Deallocates memory
		~LTexture();
		
		//Allocate texture
		bool capture();
		
		//Deallocates texture
		void free();
		
		//Set color modulation
        void setColour( Uint8 red, Uint8 green, Uint8 blue );
        
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
				
	private:
		//texture representor for hardware
		SDL_Texture* _Texture;
		
		//Image dimensions
		int _Width;
		int _Height;
};

//initializer
bool init();

//closes media and SDL
void close( );

//window being used
SDL_Window* gWindow = NULL;

//window renderer
SDL_Renderer* gRenderer = NULL;


bool init() {
	//initialize success flag
	bool success = true;
	
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}
	
	return success;
}

void close( ) {
	//free loaded images
	
	
	//destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	
	//Quit SDL
	IMG_Quit();
	SDL_Quit();
}

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
	
	//Color key image
//	SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );
		
	//Create texture from surface pixels
	newTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET, 200, 200 );
		
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
