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
#include <string>
#include <sstream>
#include <utility>
namespace jimdb
{
    namespace common
    {

        enum LoggerTypes
        {
            ERROR_L = 0,
            EXCAPTION = 1,
            WARNING = 2,
            TIMER = 3,
            INFO = 4,
            DEBUG = 5,
            SIZE_OF_ENUM_LOGGER_TYPES
        };

        struct LoggerTypeMap
        {
            static const char* EnumString[];
            /**
            @return the configValue as const char*
            */
            static std::string get(const LoggerTypes& e);
        };

        /**
        \brief Regular LogMessage

        A regular Logmessage which will be written to Logger when leaving the scope.
        Use \see Logger::Error() to get a Message. Do "create" this without the Logger class!
        \author Benjamin Meyer
        \date
        */
        class LogMessage
        {
        public:
            /**
            \brief Ctor for the a LogMessage

            It takes several parameter to log the position in file and line in file.

            \param[in] type
            \param[in] file
            \param[in] i
            \author Benjamin Meyer
            \date 22.05.2015 16:05
            */
            explicit inline LogMessage(const LoggerTypes& type, const std::string& file = "",
                                       const int& i = 0);
            ~LogMessage();
            std::string currentDateTime() const;
            /**
            returns self reference
            */
            template <typename T>
            inline LogMessage& operator<<(const T& m);

            template <typename T>
            inline LogMessage& operator<<(T& m);


            LogMessage(const LogMessage& other);
            LogMessage(LogMessage&& other);

            LogMessage& operator=(const LogMessage& other);
            LogMessage& operator=(LogMessage&& other);

        private:
            std::ostringstream m_stream;
            LoggerTypes m_type;
        };
    }
}
#include "logmessage.hpp"