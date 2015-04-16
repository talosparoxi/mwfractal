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
	LTexture gTexture;
	if (gTexture.capture()){
	};
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
//	int palette_size = this->_palette.size() - 1;

//    PixelPacket *pixel_cache = this->_image.getPixels( 0, 0, this->_px, this->_py );
//    PixelPacket *next_pixel = pixel_cache;

	gTexture.setColour(128, 128, 128);

/*

    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
        for( this->_idx = 0; this->_idx < this->_px; this->_idx++ ) {
            if( (*this->results)[this->_idy][this->_idx] != -1 ) {
                if( this->_opts->invertspectrum ) {
//                    *next_pixel = this->_palette[palette_size - ( int )floor( ( (*this->results)[this->_idy][this->_idx] - this->_lo_iteration ) * this->_colour_scaler )];
                } else {
//                    *next_pixel = this->_palette[( int )floor( ( (*this->results)[this->_idy][this->_idx] - this->_lo_iteration ) * this->_colour_scaler )];
                }
            }
            *next_pixel++;
        }
        this->_current_iteration += this->_px;
        this->_temp = floor( this->_current_iteration / this->_progress_diff );
        if( this->_temp > this->_progress ) {
            while( this->_progress < this->_temp ) {
                this->_progress++;
                cout << ".";
            }
            cout.flush();
        }
    }

//    this->_image.syncPixels();

    cout << endl << endl << "Completed " << this->_total_iterations << " pixels" << endl;
    cout.flush();
*/
	
    return true;
}

void Colourizer::writeImage( const char* filename ) {
//    this->_image.write( filename );
}

