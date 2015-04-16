#include <SDL.h>

class LTexture;

//texture to be used
extern LTexture gTexture;

//initializer
bool init();

//closes media and SDL
void close( );

//window being used
extern SDL_Window* gWindow;

//window renderer
extern SDL_Renderer* gRenderer;

