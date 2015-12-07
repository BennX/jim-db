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
            tasking::RWLockGuard<> lock(m_lock, tasking::WRITE);
            m_index[k] = type;//at to regular index

            //insert into the "last idx"
            m_freePages[k] = type;
        }


        std::shared_ptr<memorymanagement::Page> PageIndex::find(const size_t& free)
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::WRITE);
            //now find right but backwards
            for (auto it = m_freePages.rbegin(); it != m_freePages.rend();)
            {
                //if the page has a chunk where i can compleatly fit
                // faster then try insert and revert if not fit
                // else free() > free for a try insert lateron

                if(it->second->full())
                {
                    //the page is full erase it from the "free pages"
                    m_freePages.erase(it++);
                    continue;
                }
                //check the others
                if (!it->second->isLocked() && it->second->free(free))
                {
                    return it->second; //dont unlock when returned
                }
				//increment here
				++it;
            }
            return nullptr;
        }
    }
}