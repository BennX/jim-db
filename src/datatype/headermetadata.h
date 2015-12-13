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
namespace jimdb
{
    namespace memorymanagement
    {
        /**
        \brief The header information for one object

        @author Benjamin Meyer
        @date 23.10.2015 15:27
        */
        class HeaderMetaData
        {
        public:
            inline HeaderMetaData(size_t id, size_t hash, size_t type, size_t pos);
            /**
            Dtor which set everything to 0 "kind of clean".
            @author Benjamin Meyer
            @date 23.10.2015 15:29
            */
            inline ~HeaderMetaData();
            inline size_t getOID() const;
            inline size_t getHash() const;
            inline size_t getObjektType() const;
            inline size_t getPos() const;
        private:
            //objekt id
            size_t m_oid;
            //object hash
            size_t m_hash;
            //objekt type id better say the objekt name hash
            size_t m_objektType;
            //the position in body
            size_t m_pos;
        };
    }
}
#include "headermetadata.hpp"