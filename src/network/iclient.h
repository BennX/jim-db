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
#include <memory>
#include "message.h"
namespace jimdb
{
    namespace network
    {

        class IClient
        {
        public:
            virtual ~IClient() { }

            /**
            \brief sending data to the client

            @param[in] string the message to send
            @author Benjamin Meyer
            @date 21.11.2015 18:28
            */
            virtual bool send(std::shared_ptr<std::string> s) = 0;
            virtual bool isConnected() const = 0;

            /**
            \brief get Data with a async call
            It does get data async for 1 second or throws!

            @throw error if nothing got recv or timeout
            @return a Message object of the data that got received
            @author Benjamin Meyer
            @date 21.11.2015 18:26
            */
            virtual std::shared_ptr<Message> getData() = 0;
        };
    }
}