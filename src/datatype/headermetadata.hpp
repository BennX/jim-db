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

namespace jimdb
{
    namespace memorymanagement
    {

        inline HeaderMetaData::HeaderMetaData(size_t id, size_t hash, size_t type, size_t pos): m_oid(id), m_hash(hash),
            m_objektType(type), m_pos(pos) {}

        inline HeaderMetaData::~HeaderMetaData()
        {
            //set everything to 0 on delete;
            m_oid = 0;
            m_hash = 0;
            m_objektType = 0;
            m_pos = 0;
        }

        inline size_t HeaderMetaData::getOID() const
        {
            return m_oid;
        }

        inline size_t HeaderMetaData::getHash() const
        {
            return m_hash;
        }

        inline size_t HeaderMetaData::getObjektType() const
        {
            return m_objektType;
        }

        inline size_t HeaderMetaData::getPos() const
        {
            return m_pos;
        }
    }
}