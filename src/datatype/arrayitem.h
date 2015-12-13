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
        enum ETypes
        {
            INT,
            BOOL,
            OBJECT,
            ARRAY,
            DOUBLE,
            STRING,
            INVALID
        };

        /**
        \brief ArrayItem type

        this is a "container" to optain the type of an array.
        If we got the type we can stancil it with the BaseType
        according to the type. OOP is a nice thing here ;)

        Always take the according constructor while creation

        later just cast it to any template and get the type to cast
        it to the according arrayItem with the right type
        @author Benjamin Meyer
        @date 28.10.2015 16:37
        */
        template<typename T>
        class ArrayItem : public BaseType<T>
        {
        public:

            /**
            \brief Constructor for example object

            @author Benjamin Meyer
            @date 28.10.2015 17:59
            */
	        explicit ArrayItem(T t, ETypes e);
            inline ETypes getType() const;

            /**
            \brief Set the type used for strings and so on

            Since strings cant be constructed with the basetype
            @author Benjamin Meyer
            @date 28.10.2015 17:52
            */
            inline void setType(ETypes type);
        private:
            ETypes m_type;
        };
#include "arrayitem.hpp"
    }
}