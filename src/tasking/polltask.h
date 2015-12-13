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
#pragma once
#include "itask.h"
#include <chrono>
#include "../network/asiohandle.h"

namespace jimdb
{
    namespace tasking
    {
        enum PollType
        {
            HANDSHAKE,
            RECEIVE
        };

        class PollTask : public ITask
        {
        public:
	        bool continuous() override;
	        explicit PollTask(std::shared_ptr<network::AsioHandle> sock, const PollType& p, const int& timeout = 10000);
            ~PollTask() override;
            void operator()() override;

        private:
            std::chrono::steady_clock::time_point m_last;
            int m_timeout;
            char* m_buffer;
            bool m_active;
            PollType m_type;

			static int m_counter;
        };
    }
}