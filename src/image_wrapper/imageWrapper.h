#include <SDL.h>
#include "..\ProgramOptions.hpp"

#ifndef Texture
#define Texture

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		bool createForWindow( );
		
		bool createForPixelData( int w, int h );
		
		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x = 0, int y = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		//Pixel manipulators
		bool lockTexture();
		bool unlockTexture();
		void* getPixels();
		int getPitch();

	private:
		//The actual hardware texture
		SDL_Texture* _texture;
		void* _pixels;
		int _pitch;

		//Image dimensions
		int _width;
		int _height;
};

extern int windowWidth;
extern int windowHeight;



#endif
//LTexture gTexture = new LTexture; 
//LTexture gTexture;
//extern LTexture* gTexture = new LTexture; 
extern LTexture* gTexture;

//initializer
bool init();

//closes media and SDL
void close( );

//window being used
extern SDL_Window* gWindow;
extern SDL_Surface* screenSurface;

//window renderer
extern SDL_Renderer* gRenderer;

