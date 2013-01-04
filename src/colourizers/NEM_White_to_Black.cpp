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

#include "NEM_White_to_Black.hpp"

using namespace JS;
using namespace Magick;
using namespace std;

NEM_White_to_Black::NEM_White_to_Black( boost::shared_ptr<ProgramOptions> opts )
 : Colourizer::Colourizer( opts ) {
}

NEM_White_to_Black::NEM_White_to_Black( const NEM_White_to_Black& orig )
 : Colourizer::Colourizer( orig ) {
}

NEM_White_to_Black::~NEM_White_to_Black() {
}

bool NEM_White_to_Black::generatePalette() {
    for( this->_idx = 0; this->_idx < this->_opts->number_hue; this->_idx++ ) {
        if( this->_opts->colour_weighting == 0.0 ) {
            this->_l = ( double ) this->_idx / this->_opts->number_hue;
        } else if( this->_opts->colour_weighting > 0.0 ) {
            this->_l = atan( this->_idx * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler;
        } else {
            this->_l = 1.0 - atan( ( this->_opts->number_hue - this->_idx ) * this->_arctan_horiz_scaler ) / this->_arctan_vert_scaler;
        }

        this->_r = this->_l;
        this->_g = this->_l;
        this->_b = this->_l;

        this->_palette[this->_idx] = ColorRGB( this->_r, this->_g, this->_b );
    }

    return true;
}

