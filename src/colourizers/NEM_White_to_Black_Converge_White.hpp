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

#ifndef NEM_WHITE_TO_BLACK_CONVERGE_WHITE_HPP
#define	NEM_WHITE_TO_BLACK_CONVERGE_WHITE_HPP

#include "NEM_White_to_Black.hpp"

namespace JS {

class NEM_White_to_Black_Converge_White : public NEM_White_to_Black {

public:
    NEM_White_to_Black_Converge_White( boost::shared_ptr<ProgramOptions> opts );
    NEM_White_to_Black_Converge_White( const NEM_White_to_Black_Converge_White& orig );
    virtual ~NEM_White_to_Black_Converge_White();
    bool preRun();

private:

};

}

#endif
