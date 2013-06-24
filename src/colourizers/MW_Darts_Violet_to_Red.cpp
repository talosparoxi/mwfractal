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

#include "MW_Darts_Violet_to_Red.hpp"

using namespace JS;
using namespace Magick;
using namespace std;

MW_Darts_Violet_to_Red::MW_Darts_Violet_to_Red(  boost::shared_ptr<ProgramOptions> opts )
 : Colourizer::Colourizer( opts ) {
	srand( time( NULL ) );
} 

MW_Darts_Violet_to_Red::MW_Darts_Violet_to_Red( const MW_Darts_Violet_to_Red& orig )
 : Colourizer::Colourizer( orig ) {
}

MW_Darts_Violet_to_Red::~MW_Darts_Violet_to_Red() {
}

bool MW_Darts_Violet_to_Red::generatePalette() {
	this->_palette_progress_diff = (float)this->_opts->number_lightness / 80;
    for( this->_idy = 0; this->_idy <= this->_opts->number_lightness; this->_idy++ ) {
        for( this->_idx = 0; this->_idx <= this->_opts->number_hue; this->_idx++ ) {
            if( this->_opts->colour_weighting == 0.0 ) {
                this->_h = this->_spectral_diff * this->_idx / this->_opts->number_hue + this->_opts->spectral_min;
            } else if( this->_opts->colour_weighting > 0.0 ) {
                this->_h = this->_spectral_diff * atan( this->_idx * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler + this->_opts->spectral_min;
            } else {
                this->_h = this->_spectral_diff * ( 1.0 - atan( ( this->_opts->number_hue - this->_idx ) * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler ) + this->_opts->spectral_min;
            }

            this->_s = 1.0;
            this->_l = this->_lightness_diff / ( this->_opts->number_lightness - 1 ) * ( this->_idy - 1 ) + this->_opts->lightness_min;

            this->_chroma = ( 1.0 - fabs( 2.0 * this->_l - 1.0 ) ) * this->_s;
            this->_x = this->_chroma * ( 1.0 - fabs( fmod( this->_h, 2.0 ) - 1.0 ) );

            if( ( 0.0 <= this->_h ) && ( this->_h < 1.0 ) ) {
                this->_r = this->_chroma;
                this->_g = this->_x;
                this->_b = 0;
            } else if( ( 1.0 <= this->_h ) && ( this->_h < 2.0 ) ) {
                this->_r = this->_x;
                this->_g = this->_chroma;
                this->_b = 0;
            } else if( ( 2.0 <= this->_h ) && ( this->_h < 3.0 ) ) {
                this->_r = 0;
                this->_g = this->_chroma;
                this->_b = this->_x;
            } else if( ( 3.0 <= this->_h ) && ( this->_h < 4.0 ) ) {
                this->_r = 0;
                this->_g = this->_x;
                this->_b = this->_chroma;
            } else {
                this->_r = this->_x;
                this->_g = 0;
                this->_b = this->_chroma;
            }

            this->_m = this->_l - this->_chroma / 2.0;
            this->_r = this->_r + this->_m;
            this->_g = this->_g + this->_m;
            this->_b = this->_b + this->_m;

            this->_palette[this->_idx + this->_idy * this->_opts->number_hue] = ColorRGB( this->_r, this->_g, this->_b );
        }
		this->paletteProgressTick( this->_idy );
    }

    return true;
}

bool MW_Darts_Violet_to_Red::run() {
	int palette_size = this->_palette.size() - 1;

    PixelPacket *pixel_cache = this->_image.getPixels( 0, 0, this->_px, this->_py );
    PixelPacket *next_pixel = pixel_cache;

    for( this->_idy = 0; this->_idy < this->_py; this->_idy++ ) {
        for( this->_idx = 0; this->_idx < this->_px; this->_idx++ ) {
            if( (*this->results)[this->_idy][this->_idx] != -1 ) {
					this->_frac_part = this->game();
					if( this->_opts->invertspectrum ) {
						*next_pixel = this->_palette[palette_size - ( int )floor( ( (*this->results)[this->_idy][this->_idx] - this->_lo_iteration ) * this->_colour_scaler ) + this->_opts->number_hue * ( int )floor( this->_opts->number_lightness * this->_frac_part )];
					} else {
						*next_pixel = this->_palette[( int )floor( ( (*this->results)[this->_idy][this->_idx] - this->_lo_iteration ) * this->_colour_scaler ) + this->_opts->number_hue * ( int )floor( this->_opts->number_lightness * this->_frac_part )];
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
    this->_image.syncPixels();

    cout << endl << endl << "Completed " << this->_total_iterations << " pixels" << endl;
    cout.flush();
	
    return true;
}

float MW_Darts_Violet_to_Red::game() {
	int score = 0;
	vector<complex<float> >::iterator orbit_itr;
	for( orbit_itr = this->orbits->at(this->_idy).at(this->_idx).begin(); orbit_itr != this->orbits->at(this->_idy).at(this->_idx).end(); orbit_itr++ ) {
		score += this->getScore( (*orbit_itr) );
	}

	return (float)score / ( this->orbits->at(this->_idy).at(this->_idx).size() * 50 );
}

int MW_Darts_Violet_to_Red::getScore( std::complex<float> shot ) {
	float phase_angle = arg( shot );

	float bullseye = 0.5;

	int score = 0;

	if( abs( shot ) > 26.5 * bullseye ) return 0;
	if( abs( shot ) < bullseye ) return 50;
	if( abs( shot ) < 2.5 * bullseye ) return 25;


	if( phase_angle <= -0.95 * M_PI ) {
		score = 11;
	} else if( phase_angle <= -0.85 * M_PI ) {
		score =  8;
	} else if( phase_angle <= -0.75 * M_PI ) {
		score =  16;
	} else if( phase_angle <= -0.65 * M_PI ) {
		score =  7;
	} else if( phase_angle <= -0.55 * M_PI ) {
		score =  19;
	} else if( phase_angle <= -0.45 * M_PI ) {
		score =  3;
	} else if( phase_angle <= -0.35 * M_PI ) {
		score =  17;
	} else if( phase_angle <= -0.25 * M_PI ) {
		score =  2;
	} else if( phase_angle <= -0.15 * M_PI ) {
		score =  15;
	} else if( phase_angle <= -0.05 * M_PI ) {
		score =  10;
	} else if( phase_angle <= 0.05 * M_PI ) {
		score =  6;
	} else if( phase_angle <= 0.15 * M_PI ) {
		score =  13;
	} else if( phase_angle <= 0.25 * M_PI ) {
		score =  4;
	} else if( phase_angle <= 0.35 * M_PI ) {
		score =  18;
	} else if( phase_angle <= 0.45 * M_PI ) {
		score =  1;
	} else if( phase_angle <= 0.55 * M_PI ) {
		score =  20;
	} else if( phase_angle <= 0.65 * M_PI ) {
		score =  5;
	} else if( phase_angle <= 0.75 * M_PI ) {
		score =  12;
	} else if( phase_angle <= 0.85 * M_PI ) {
		score =  9;
	} else if( phase_angle <= 0.95 * M_PI ) {
		score =  14;
	} else {
		score =  11;
	}

	//dblring 25
	//outside treble 16.5
	//inside treble 15
	if( abs( shot ) > 25 * bullseye ) return score * 2;
	if( ( abs( shot ) > 15 * bullseye ) && ( abs( shot ) < 16.5 * bullseye ) ) return score * 3;


	return score;
}
