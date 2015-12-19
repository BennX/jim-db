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
#include "../page/page.h"
#include "index.h"
#include "../thread/spinlock.h"
#include <map>
#include <vector>

namespace jimdb
{
    namespace index
    {
        /**
        \brief Singleton PageIndex

        It contains a Bucketing system which can be setup by the configuration.
        Freepages get sorted into buckets.

        Whenever a page is generated, which only happens inside the insert task, it does
        get pushed to free after using. If it is inside the "free" it is sorted to buckets depending
        on the free value of the Page. If the freevalue is to small it does not get sorted in and
        only remains inside the regular index which does not get searched for free pages.

        @author Benjamin Meyer
        @date 23.10.2015 12:08
        */
        class PageIndex: public Index<size_t, std::shared_ptr<memorymanagement::Page>>
        {
        public:
            static PageIndex& getInstance();
            /**
            \brief need to be called right after initing
            	the configuration. Without configuration the page
            	index cant be inited correct.
            	It does init the buckets of the Index with the set index values.
            	The first bucket is guaranteed to be empty.
            	the last bucket always contains the newest pages.

            @author Benjamin Meyer
            @date 18.12.2015 09:47
            */
            void init();

            /**
            \brief add a page back to the Buckets

            If a page is in the free Buckets it gets iterated while checking for a
            free page.

            If the page Freevalue is to small for a bucket, it does
            not get added to it. It only get added if there is a fitting bucket for it.

            Like so: if the Page has 560 Free and there is a 512 Bucket it get added to it but
            not to the 1024 Bucket!

            If the page free is smaler then the lowest bucket it does not get added to any bucket and
            remains in the regular index.

            @author Benjamin Meyer
            @date 14.12.2015 19:42
            */
            inline void pushToFree(const VALUE& type);

            /**
            \brief find a page and return it from the buckets.

            If it is returned the Page is not inside the "free" buckets anymore
            therefore it is not searched of other insert tasks.

            If searching for a free page it is looking inside the next
            BIGGER bucket. If there is no entry it does look in the next bigger bucket.
            If there is still no page it does return null inside of the find method.

            @param[in] free the size needed
            @return nullptr or a page which can fit the objekts

            @author Benjamin Meyer
            @date 01.11.2015 11:33
            */
            inline std::shared_ptr<memorymanagement::Page> find(const size_t& free);

        private:
            PageIndex();

            tasking::SpinLock m_findSpin;

            static PageIndex m_instance;
            //first <bucket,index<id,page>>
            std::map<uint64_t, std::vector<std::shared_ptr<memorymanagement::Page>>> m_freePages;
        };
    }
}
#include "pageindex.hpp"