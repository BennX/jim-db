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
#include <memory>

namespace jimdb
{
    namespace memorymanagement
    {
        /**
        \brief The BaseType Typ Class
        It is templated. Use it with care!
        the minimum size of the data filed is 64bit (size field used for String type). Do not make it bigger
        by taking a huge template parameter! Simply use double / long long / bool
        as needed for JSON.

        \author Benjamin Meyer
        \date 25.09.2015 09:22
        */
        template <typename T>
        class BaseType
        {
        public:
            BaseType();
            explicit BaseType(T& t);
            explicit BaseType(const T& t);

            ~BaseType();

            //inlines not realy needed since the template forces inline
            inline void setNext(const ptrdiff_t& next);
            inline std::ptrdiff_t getNext();
            inline void setData(T& t);
            inline void setData(const T& t);
            inline T getData() const;

        protected:
            union DataUnion
            {
                T data;
                ::std::ptrdiff_t size;

                DataUnion()
                {
                    memset(this, 0, sizeof(DataUnion));
                } //init with 0
                explicit DataUnion(T& t);
                explicit DataUnion(const T& t);
            } m_data;

            long long m_next;
        };
#include "BaseType.hpp"
    }
}
