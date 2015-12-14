#include <sstream>
#include "../log/logger.h"

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

namespace jimdb
{
    namespace index
    {
        template <typename KEY, typename VALUE>
        inline void Index<KEY, VALUE>::add(const KEY& name, const VALUE& type)
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::WRITE);
            m_index[name] = type;
        }

        template <typename KEY, typename VALUE>
        inline bool Index<KEY, VALUE>::contains(const KEY& name)
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::READ);
            if (m_index.count(name))
                return true;
            return false;
        }

        template <typename KEY, typename VALUE>
        inline bool Index<KEY, VALUE>::erase(const KEY& name)
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::WRITE);
            if (m_index.count(name)) //dont use contains! lock in lock is bad!
            {
                m_index.erase(name);
                return true;
            }
            return false;
        }

        template <typename KEY, typename VALUE>
        inline const VALUE& Index<KEY, VALUE>::operator[](const KEY& name)
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::READ);
            return m_index[name];
        }

        /** // simply dont do this!
        template <typename KEY, typename VALUE>
        inline const stx::btree_map<KEY, VALUE>& Index<KEY, VALUE>::get()
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::READ);
            return m_index;
        }
        */

        template <typename KEY, typename VALUE>
        inline bool Index<KEY, VALUE>::empty()
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::READ);
            return m_index.empty();
        }

        template <typename T, typename U>
        stx::btree_map<typename Index<T, U>::KEY, typename Index<T, U>::VALUE>& Index<T, U>::get()
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::READ);
            return m_index;
        }

        template <typename T, typename U>
        void Index<T, U>::print()
        {
            tasking::RWLockGuard<> lock(m_lock, tasking::READ);
            std::stringstream ss;
            for (auto it = m_index.begin();
                    it != m_index.end(); ++it)
            {
                ss << ";" << it->first;
            }
            LOG_DEBUG << ss.str();
        }
    }
}