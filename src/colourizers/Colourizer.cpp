/*  Copyright 2013 Neil E. Moore, Christopher J. Willcock

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */

#include "Colourizer.hpp"

#include "../utils.hpp"

using namespace JS;
//using namespace Magick;
using namespace std;

int streamManip(std::string colour, int pos, int length){
	std::stringstream ss;
	
	ss << std::hex << colour.substr(pos, length);
	ss >> colour;
	return boost::lexical_cast<int>( colour );
}

Colourizer::Colourizer( boost::shared_ptr<ProgramOptions> opts ) {
    this->_opts = opts;
    
    this->_spectral_diff = opts->spectral_max - opts->spectral_min;
    this->_lightness_diff = opts->lightness_max - opts->lightness_min;
    this->_arctan_horiz_scaler = opts->colour_weighting / opts->number_hue;
    this->_arctan_vert_scaler = atan( opts->colour_weighting );


    this->_px = opts->width;
    if( this->_px == 0 ) {
		this->_px = 1;
	}
    this->_py = opts->height;
    if( this->_py == 0 ) {
		this->_py = 1;
	}
    this->_total_iterations = this->_px * this->_py;
    this->_current_iteration = 0;
    this->_progress_diff = (float)this->_total_iterations / 80;
    this->_progress = 0;
    this->_palette_progress_diff = (float)opts->number_hue / 80;
    this->_palette_progress = 0;
    this->_lo_iteration = 0xFFFFFFFF;
    this->_hi_iteration = 0;
    if( !init() ) {
	}
	
//	this->_image = Image( Geometry( this->_px, this->_py ), opts->convergecolour );
//    this->_image.type( TrueColorType );



}

Colourizer::Colourizer( const Colourizer& orig ) {
}

Colourizer::~Colourizer() {
}

void Colourizer::setResults( std::vector<std::vector<float> >* results ) {
    this->results = results;
    std::vector<float>::iterator min_temp, max_temp;
    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
        min_temp = min_element( (*this->results)[this->_idy].begin(), (*this->results)[this->_idy].end(), findmin );
        max_temp = max_element( (*this->results)[this->_idy].begin(), (*this->results)[this->_idy].end() );
		if( this->_lo_iteration > (*min_temp) ) {
            this->_lo_iteration = (*min_temp);
        }
        if( this->_hi_iteration < (*max_temp) ) {
            this->_hi_iteration = (*max_temp);
        }
    }
    // scale to set numbers between the max and min colour values
    this->_colour_scaler = ( this->_opts->number_hue - 1 ) / ( this->_hi_iteration - this->_lo_iteration );
}

void Colourizer::setOrbits( std::vector<std::vector<std::vector<std::complex<float> > > >* orbits ) {
	this->orbits = orbits;
}

bool Colourizer::generatePalette() {
    return false;
}

bool Colourizer::paletteProgressTick( int current ) {
        this->_temp = ceil( current / this->_palette_progress_diff );
        if( this->_temp > this->_palette_progress ) {
            while( this->_palette_progress < this->_temp ) {
                this->_palette_progress++;
                cout << ".";
            }
            cout.flush();
        }

		return true;
}

bool Colourizer::run() {
	gTexture->createForPixelData(this->_opts->width, this->_opts->height);
	
    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
    	if( !gTexture->lockTexture() ) {
					printf( "Unable to lock texture!\n" );
					} else{
        for( this->_idx = 0; this->_idx < this->_px; this->_idx++ ) {
            if( (*this->results)[this->_idy][this->_idx] != -1 ) {
			//colour declaration
				int _colour = ( (*this->results)[this->_idy][this->_idx] - this->_lo_iteration ) * this->_colour_scaler;
				int hexScaler = 0xFFFFFF / (this->_opts->number_hue);
				
				int hexValue;
				
				hexValue = ( int )floor( _colour * hexScaler );
				_r = (hexValue & 0xFF0000) >> 16;
				_g = ( hexValue & 0x00FF00) >> 8;
				_b = hexValue & 0x0000FF;
//				cout << (_colour / this->_colour_scaler ) << _colour << (hexValue & 0xFFFFFF) << "   " << _r << "    " << _g << "    " << _b << endl;
				/*
				std::string _colourHex;
			
				std::stringstream ss;
				ss << std::dec << _colourDec;
				ss >> _colourHex;
				
				switch(_colourHex.length()) {
					case 1:
						_r = streamManip( _colourHex, 0, 1 );
						break;
					case 2:
						_r = streamManip( _colourHex, 0, 2 );
						break;
					case 3:
						_r = streamManip( _colourHex, 0, 2 );
						_g = streamManip( _colourHex, 2, 1 );
						break;
					case 4:
						_r = streamManip( _colourHex, 0, 2 );
						_g = streamManip( _colourHex, 2, 2 );
						break;
					case 5:
						_r = streamManip( _colourHex, 0, 2 );
						_g = streamManip( _colourHex, 2, 2 );
						_b = streamManip( _colourHex, 4, 1 );
					break;
					default:
						_r = streamManip( _colourHex, 0, 2 );
						_g = streamManip( _colourHex, 2, 2 );
						_b = streamManip( _colourHex, 4, 2 );
					break;
				}
				*/
//				SDL_Rect fillRect = { this->_idx, this->_idy, 1, 1 };	
////				//set pixel color	
//				if( this->_opts->invertspectrum ) {
//					SDL_SetRenderDrawColor( gRenderer, 255-_r, 255-_g, 255-_b, 0 );	
//				} else {
//					SDL_SetRenderDrawColor( gRenderer, _r, _g, _b, 0 );	
//				}
//
////				if(this colour is less than previous iterations){
////					SDL_RenderFillRect( gRenderer, &fillRect );
////				}
//				SDL_RenderFillRect( gRenderer, &fillRect );
//            }
//				
				//Lock texture
				
				
					//Get pixel data
					Uint32* pixels = (Uint32*)gTexture->getPixels();
					int pixelCount = this->_idx + ( this->_px * this->_idy );
//					if( SDL_MapRGBA( SDL_GetWindowSurface( gWindow )->format, _r, _g, _b, 0 ) > pixels[ pixelCount ] ) {
					pixels[ pixelCount ] = SDL_MapRGBA( SDL_GetWindowSurface( gWindow )->format, (_palette[ _colour ]).r , ( _palette[ _colour ]).g, (_palette[ _colour ]).b , (_palette[ _colour ]).a );
//				}
//					pixels[ pixelCount ] = SDL_MapRGBA( SDL_GetWindowSurface( gWindow )->format, _r, _g, _b, 0 );
						
//		cout<<(_palette[_colour].r)<< " /  " <<(_palette[_colour]).g << "  /  " << _palette[_colour].b <<endl;
					}
		
					
				}
				
				
				
        }
        //Unlock texture
		gTexture->unlockTexture();
        this->_current_iteration += this->_px;
        this->_temp = floor( this->_current_iteration / this->_progress_diff );
        if( this->_temp > this->_progress ) {
            while( this->_progress < this->_temp ) {
                this->_progress++;
                cout << ".";
                gTexture->render();
                SDL_RenderPresent( gRenderer );
            }
            cout.flush();
        }
    }
//Update the surface
	gTexture->render();
	SDL_RenderPresent( gRenderer );
	SDL_Delay( 8000 );
    cout << endl << endl << "Completed " << this->_total_iterations << " pixels" << endl;
    cout.flush();

	
    return true;
}

void Colourizer::writeImage( const char* filename ) {
//	unsigned char * pixels = new (std::nothrow) unsigned char[gWindow->w * gWindow->h * gWindow->format->BytesPerPixel];
//	SDL_RenderReadPixels(gRenderer, &gWindow->clip_rect, gWindow->format->format, pixels, gWindow->w * gWindow->format->BytesPerPixel);
//  SDL_Surface* saveSurface = SDL_CreateRGBSurfaceFrom(pixels, gWindow->w, gWindow->h, gWindow->format->BitsPerPixel, gWindow->w * gWindow->format->BytesPerPixel, gWindow->format->Rmask, gWindow->format->Gmask, gWindow->format->Bmask, gWindow->format->Amask);
                       
//	SDL_Surface* screenSurface = SDL_GetWindowSurface( gWindow );;


//	SDL_UpdateWindowSurface(gWindow)
//	SDL_SaveBMP( saveSurface, filename );
}

