#include "../common/configuration.h"

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
    namespace index
    {

        void PageIndex::pushToFree(const VALUE& type)
        {
            std::lock_guard<tasking::SpinLock> lock(m_findSpin);

            //insert into the most upper bound index. Guaranteed to be log(n)
            auto it = m_freePages.upper_bound(static_cast<uint64_t>(type->free()));

            //only push if there is a bucket
            if(it != m_freePages.begin())
            {
                //since it is ordered we can simple call -- to get the
                //next lower element.
                --it;
                //push it back
                it->second.push_back(type);
            }
            else
            {
                //if we have the first bucket only push it if it fits
                if (type->free() > it->first)
                    it->second.push_back(type);
            }

        }


        std::shared_ptr<memorymanagement::Page> PageIndex::find(const size_t& free)
        {

            //write lock it since we meight delete something
            std::lock_guard<tasking::SpinLock> lock(m_findSpin);

            //if we do not find something in the right bound go to the upper
            for (auto l_bucketIt = m_freePages.upper_bound(free); l_bucketIt != m_freePages.end(); ++l_bucketIt)
            {
                //now find right but backwards
                for (auto it = l_bucketIt->second.begin(); it != l_bucketIt->second.end(); ++it)
                {
                    //check the others
                    if ((*it)->free(free))
                    {
                        //we found a page to fit it
                        auto l_ret = *it;
                        //delete it from the freepage list
                        l_bucketIt->second.erase(it);
                        return l_ret;
                    }
                    //break if we get here and got the last element
                }
            }
            return nullptr;
        }
    }
}