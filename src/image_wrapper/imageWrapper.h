#include <SDL.h>

#ifndef Texture
#define Texture

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

