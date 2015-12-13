// /**
// ############################################################################
// # GPL License                                                              #
// #                                                                          #
// # This file is part of the JIM-DB.                                         #
// # Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
// # This program is free software: you can redistribute it and/or modify     #
// # it under the terms of the GNU General Public License as                  #
// # published by the Free Software Foundation, either version 3 of the       #
// # License, or (at your option) any later version.                          #
// #                                                                          #
// # This program is distributed in the hope that it will be useful,          #
// # but WITHOUT ANY WARRANTY; without even the implied warranty of           #
// # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
// # GNU General Public License for more details.                             #
// #                                                                          #
// # You should have received a copy of the GNU General Public License        #
// # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
// ############################################################################
// **/
#pragma once
#include "basetype.h"

namespace jimdb
{
    namespace memorymanagement
    {
        /**
        \brief Simple stancil for ArrayType

        It is not needed as we could use the basetype 
		and just optain the data but this is more nice.
        @author Benjamin Meyer
        @date 28.10.2015 16:46
        */
        class ArrayType: public BaseType<size_t>
        {
        public:
			explicit inline ArrayType(size_t s);
            inline size_t size() const;
		};
    }
}
#include "arraytype.hpp"