﻿/**
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
#include <memory>
#include <string>
#include <cstdint>
#include <rapidjson/document.h>
namespace jimdb
{
    namespace network
    {
        enum MessageTypes
        {
            HANDSHAKE,
            RESULT,
            ERROR_L,
            WHAT,
            ENUM_SIZE,
        };

        struct MessageTypeMap
        {
            //need the strings for the size
            static const char* EnumString[];

            /**
            @return the configValue as const char*
            */
            static const char* get(const MessageTypes& e);
        };

        /**
        \brief Prototyp of a message Factory

        just a prototyp to use now
        \author Benjamin Meyer
        \date 02.10.2015 18:23
        **/
        class MessageFactory
        {
        public:
            MessageFactory();
            std::shared_ptr<std::string> handshake();
            std::shared_ptr<std::string> error(const std::string& what);

            /**
            \brief generates a result message of an insert

            @param[in] oid the object ID which was inserted
            @author Benjamin Meyer
            @date 01.11.2015 09:42
            */
            std::shared_ptr<std::string> generateResultInsert(const uint64_t& oid);
        private:
            std::shared_ptr<std::string> generate(const MessageTypes& t, rapidjson::Value& data);
            std::shared_ptr<std::string> toString(rapidjson::Value& data) const;
        };
    }
}