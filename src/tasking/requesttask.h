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
#include "../bench/bench.h"
#include "itask.h"
#include "../network/message.h"

namespace jimdb
{
    namespace tasking
    {
        /**
        \brief This task accepts a new request

        It comes right after the handshake and does get the data itself to process.
        Moreover it does check which type it is and generates the depending task for it.
        \author Benjamin Meyer
        \date 02.10.2015 16:20
        */
        class RequestTask : public ITask
        {
        public:


	        explicit RequestTask(const std::shared_ptr<asio::ip::tcp::socket>& sock, const std::shared_ptr<network::Message> msg);

            void operator()() override;

		private:
			Bench* m_bench;
            std::shared_ptr<network::Message> m_msg;
        };
    }
}
