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

#include "logmessage.h"
namespace jimdb
{
    namespace common
    {
		LogTimer::LogTimer(const LoggerTypes& type, const std::string& file,
			const int& i) : LogMessage(type, file, i),
			m_start(std::chrono::high_resolution_clock::now()) {}

        template <typename T>
        LogTimer& LogTimer::operator<<(const T& m)
        {
            LogMessage::operator<<(m);
            return *this;
        }


        inline LogTimer::LogTimer(const LogTimer& other) : LogMessage(other), m_start(other.m_start) { }

        inline LogTimer::LogTimer(LogTimer&& other) : LogMessage(std::move(other)), m_start(std::move(other.m_start)) { }

        inline LogTimer& LogTimer::operator=(const LogTimer& other)
        {
            if (this == &other)
                return *this;
            LogMessage::operator =(other);
            m_start = other.m_start;
            return *this;
        }


        inline LogTimer& LogTimer::operator=(LogTimer&& other)
        {
            if (this == &other)
                return *this;
            LogMessage::operator =(std::move(other));
            m_start = std::move(other.m_start);
            return *this;
        }
    }
}