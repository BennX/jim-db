/**
############################################################################
# GPL License                                                              #
#                                                                          #
# This file is part of the JIM-DB.                                         #
# Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as                  #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################
**/

#pragma once
#include "basetype.h"
#include <memory>
#include <string>
namespace jimdb
{
    namespace memorymanagement
    {
        /**
        \brief The String Type for throwing strings into memory

        This type extends the BaseType to add Strings to memory.
        It actually uses the BaseType<int> to store the size of the string.
        \author Benjamin Meyer
        \date 29.09.2015 10:45
        */

        class StringType : public BaseType<long long>
        {
        public:
            explicit inline StringType();
            explicit inline StringType(const std::string& s);
            inline std::shared_ptr<std::string> getString() const;

        protected:
            //simply dont call it! else the dtor of the base
            //set the size to 0 which would be wrong!
            //let it as it is to show that there is free space to use
            //just chain it at the end of the free space type
            ~StringType() {};
        };
#include "StringType.hpp"
    }
}
