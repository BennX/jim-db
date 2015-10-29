﻿// /**
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
#include "../page/page.h"
#include "index.h"
#include "../thread/spinlock.h"

namespace jimdb
{
    namespace index
    {
        /**
        \brief Singleton PageIndex
        @author Benjamin Meyer
        @date 23.10.2015 12:08
        */
        class PageIndex: public Index<size_t, std::shared_ptr<memorymanagement::Page>>
        {
        public:
            static PageIndex& getInstance();

            inline void add(const KEY& k, const VALUE& type);
            inline std::shared_ptr<memorymanagement::Page> last(); 
			inline std::shared_ptr<memorymanagement::Page> find(const size_t& free);
        private:
            PageIndex() : m_last(nullptr) {};
            //store the last inserted page
            //should be the "most empty"
            std::shared_ptr<memorymanagement::Page> m_last;
            static PageIndex m_instance;
        };
#include "pageindex.hpp"
    }
}