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
    namespace tasking
    {
        inline SpinLock::SpinLock()
        {
            lck.clear(std::memory_order_release);
        }

        inline void SpinLock::lock()
        {
            m_locked = true;
            while (lck.test_and_set(std::memory_order_acquire)) {}
        }

        inline void SpinLock::unlock()
        {
            lck.clear(std::memory_order_release);
            m_locked = false;
        }

        inline SpinLock::operator bool() const
        {
            return m_locked;
        }
    }
}