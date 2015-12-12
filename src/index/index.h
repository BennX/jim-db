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
#include <stx/btree_map.h>
#include "../thread/rwlock.h"
#include "../thread/rwlockguard.h"
#include "../page/page.h"
namespace jimdb
{
    namespace index
    {
        /**
        \brief a primitiv threadsafe Index Class

        A class that is threadsafe using a rwlock

        @author Benjamin Meyer
        @date 22.10.2015 13:47
        */
        template<typename T, typename U>
        class Index
        {
        public:
            typedef T KEY;
            typedef U VALUE;

            /**
            \brief add an entry to the index

            @param[in] name the name of the attribute to add. It must be unque at the object.
            there is no object that contains more than 1 attribute of the same name!
            @param[in] type the type of the attribute
            @author Benjamin Meyer
            @date 22.10.2015 12:56
            */
            inline void add(const KEY& k, const VALUE& type);

            /**
            @brief check if a attribute name exsist
            @author Benjamin Meyer
            @date 22.10.2015 12:58
            */
            inline bool contains(const KEY& name);

            /**
            @brief deletes an entry
            @return true if value could be deleted else false
            @author Benjamin Meyer
            @date 22.10.2015 12:58
            */
            inline bool erase(const KEY& name);

            /**
            \brief get a value

            @return a const reference to the value to not manipulate like
            	index[somekey].add... so its const. If the map is empty, there
            	was no entry befor calling the operator!
            @author Benjamin Meyer
            @date 22.10.2015 13:22
            */
            inline const VALUE& operator[](const KEY& k);

            /**
            @return true if the idx is empty
            @author Benjamin Meyer
            @date 31.10.2015 15:43
            */
            inline bool empty();

            /**
            \brief use this with caution!

			Since it meight get manipulated!
            @author Benjamin Meyer
            @date 02.11.2015 11:55
            */
            inline stx::btree_map<KEY, VALUE>& get();

			void print();
        protected:
            stx::btree_map<KEY, VALUE> m_index;
            tasking::RWLock m_lock;
        };
    }
}
#include "index.hpp"