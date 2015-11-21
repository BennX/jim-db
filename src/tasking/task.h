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
#include "../network/iclient.h"

namespace jimdb
{
    namespace tasking
    {
        /**
        \brief basic Task

        Holds all information that is needed and always refares to a client!
        \author Benjamin Meyer
        \date 15.09.2015 14:21
        */
        class Task
        {
        public:
            explicit Task(std::shared_ptr<network::IClient> client) :m_client(client) { };

            virtual ~Task() { }

            virtual void operator()() = 0;

        protected:
            std::shared_ptr<network::IClient> m_client;
        };
    }
}