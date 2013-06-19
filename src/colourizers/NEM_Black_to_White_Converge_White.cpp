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

#include "NEM_Black_to_White_Converge_White.hpp"

using namespace JS;
using namespace Magick;
using namespace std;

NEM_Black_to_White_Converge_White::NEM_Black_to_White_Converge_White( boost::shared_ptr<ProgramOptions> opts )
 : NEM_Black_to_White::NEM_Black_to_White( opts ) {
}

NEM_Black_to_White_Converge_White::NEM_Black_to_White_Converge_White( const NEM_Black_to_White_Converge_White& orig )
 : NEM_Black_to_White::NEM_Black_to_White( orig ) {
}

NEM_Black_to_White_Converge_White::~NEM_Black_to_White_Converge_White() {
}

bool NEM_Black_to_White_Converge_White::preRun() {
    this->_image.floodFillColor( 0, 0, "white");
    return true;
}