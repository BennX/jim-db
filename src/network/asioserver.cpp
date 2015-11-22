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


#include "asioserver.h"
#include "asioclienthandle.h"
#include "../tasking/taskqueue.h"
#include "../tasking/handshake.h"

namespace jimdb
{
    namespace network
    {
        int ASIOServer::accept(const bool& blocking)
        {
            auto l_sock = std::make_shared<asio::ip::tcp::socket>(m_io_service);
            m_acceptor.accept(*l_sock);
            auto l_client = std::make_shared<ASIOClienthandle>(l_sock);
            tasking::TaskQueue::getInstance().push_pack(std::make_shared<tasking::HandshakeTask>(l_client));
            return 0;
        }

        bool ASIOServer::start()
        {
            return false;
        }

        ASIOServer::ASIOServer() : m_io_service() ,m_acceptor(m_io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 6060)) {}

        ASIOServer::~ASIOServer() {}
    }
}