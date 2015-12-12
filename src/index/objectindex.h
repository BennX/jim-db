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
#include "index.h"

namespace jimdb
{
    namespace index
    {
        /**
        \brief Simple tupel of values

        could have used tupel but it is more intuitiv like this
        @author Benjamin Meyer
        @date 23.10.2015 11:58
        */
        class ObjectIndexValue
        {
        public:
	        ObjectIndexValue();;
            ObjectIndexValue(unsigned long long page, long long pos) : m_page(page), m_pos(pos)
            {}
            long long m_page;
            //the position of the header object!
            //not of the object itself this meight change
            //by fragmenting
            long long m_pos;
        };

        /**
        \brief Singleton object index

        @author Benjamin Meyer
        @date 23.10.2015 12:05
        */
        class ObjectIndex : public Index<size_t, ObjectIndexValue>
        {
        public:
            static ObjectIndex& getInstance();
        private:
            static ObjectIndex m_instance;
        };
    }
}