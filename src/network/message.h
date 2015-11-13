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
#include "rapidjson/document.h"

namespace jimdb
{
    namespace network
    {
        /**
        \brief Message type which holds the Message of a client

        It also holds the parsed document since it refares to the message (char*)
        and also manipulates it right after creation. ( In Situ parsing)

        \author Benjamin Meyer
        \date 01.10.2015 19:23
        */
        class Message
        {
        public:
            inline explicit Message(char* message);
            ~Message();

            /**
            \brief Method to get the document of the message

            \return A Reference to the rapidjson::Document of the message
            \author Benjamin Meyer
            \date 01.10.2015 19:24
            */
            inline rapidjson::Document& operator()();

        private:
            char* const m_message;
            rapidjson::Document m_doc;
        };

    }
}
#include "message.hpp"