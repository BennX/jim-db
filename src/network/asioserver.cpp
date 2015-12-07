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
#include "../tasking/taskqueue.h"
#include "../common/configuration.h"
#include "../tasking/polltask.h"
#include "messagefactory.h"
#include "../log/logger.h"
#include "asiohandle.h"

namespace jimdb
{
    namespace network
    {
        int ASIOServer::accept(const bool& blocking)
        {
            m_sock = std::make_shared<AsioHandle>(m_io_service);
            m_acceptor->async_accept(*m_sock, [&](asio::error_code ec)
            {
                if (ec)
                    LOG_DEBUG << ec.message();

                //write out to client
				*m_sock << MessageFactory().handshake();
                tasking::TaskQueue::getInstance().push_pack(std::make_shared<tasking::PollTask>(m_sock, tasking::PollType::HANDSHAKE));

                accept(false);
            });

            return 0;
        }

        bool ASIOServer::start()
        {
            return m_io_service.run();
        }

        ASIOServer::ASIOServer() : m_io_service() , m_acceptor(nullptr)
        {
            using namespace common;
            auto& cfg = Configuration::getInstance();
            try
            {
                std::string ip = cfg[IP].GetString() == std::string("localhost") ? "127.0.0.1" : cfg[IP].GetString();
                int port = cfg[PORT].GetInt();
                asio::ip::tcp::endpoint ep(asio::ip::address::from_string(ip), port);
                m_acceptor = std::make_shared<asio::ip::tcp::acceptor>(m_io_service, ep);
                LOG_INFO << "Server Listen on: " << ep.address().to_string() << ":" << ep.port();
            }
            catch(std::runtime_error& e)
            {
                LOG_ERROR << e.what();
            }
        }
        ASIOServer::~ASIOServer() {}
    }
}
