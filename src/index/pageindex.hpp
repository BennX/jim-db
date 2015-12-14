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
        void PageIndex::add(const KEY& k, const VALUE& type)
        {
            std::lock_guard<tasking::SpinLock> lock(m_findSpin);
            m_index[k] = type;//at to regular index

            //insert into the "last idx"
            m_freePages[k] = type;
        }

        void PageIndex::pushToFree(const KEY& k, const VALUE& type)
        {
            std::lock_guard<tasking::SpinLock> lock(m_findSpin);
            m_freePages[k] = type;
        }


        std::shared_ptr<memorymanagement::Page> PageIndex::find(const size_t& free)
        {
            //write lock it since we meight delete something
            std::lock_guard<tasking::SpinLock> lock(m_findSpin);

            std::shared_ptr<memorymanagement::Page> l_ret = nullptr;

            //now find right but backwards
            for (auto it = m_freePages.rbegin(); it != m_freePages.rend();)
            {
                //check the others
                if (it->second->free(free))
                {
                    //we found a page to fit it
                    l_ret = it->second;
                    //delete it from the freepage list
                    m_freePages.erase(l_ret->getID());
                    return l_ret;
                }
                //increment here
                ++it;
            }

            if(l_ret == nullptr)
            {
                //we didnt find any page so create one and return it
                //if the ptr is still nullptr we need to create a new Page
                //well does not fit in any page
                auto& cfg = common::Configuration::getInstance();
                l_ret = std::make_shared<memorymanagement::Page>(cfg[common::PAGE_HEADER].GetInt64(),
                        cfg[common::PAGE_BODY].GetInt64());

                //push this page to the regular index
                m_index[l_ret->getID()] = l_ret;
                return l_ret;
            }

            return nullptr;
        }
    }
}