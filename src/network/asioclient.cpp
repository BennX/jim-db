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

#include "asioclient.h"
#define MESSAGE_SIZE 8
#define WAIT_TIME 1000000 // in micoseconds
#include <future>

namespace jimdb
{
    namespace network
    {
        ASIOClient::ASIOClient(std::shared_ptr<asio::ip::tcp::socket> socket) : m_socket(socket) {}

        ASIOClient::~ASIOClient()
        {
            m_socket->close();// ? needed?
        }

        bool ASIOClient::send(std::shared_ptr<std::string> s)
        {
            char length[MESSAGE_SIZE + 1];
            sprintf(length, "%8d", static_cast<int>(s->size()));
            auto l_message = std::string(length);
            l_message += *s;
            asio::write(*m_socket, asio::buffer(l_message), asio::transfer_all());
            return true;
        }

        bool ASIOClient::hasData()
        {
            auto l_start_time = std::chrono::high_resolution_clock::now();
            long long l_dif = 0;
            while (l_dif < WAIT_TIME)
            {
                if (m_socket->available())
                {
                    return true;
                }
                l_dif = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() -
                        l_start_time).count();
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
            return false;
        }

        bool ASIOClient::isConnected() const
        {
            return true;
        }

        std::shared_ptr<Message> ASIOClient::getData()
        {
            char size[MESSAGE_SIZE + 1];
            asio::read(*m_socket, asio::buffer(size, MESSAGE_SIZE));
            auto l_size = atoi(size);
            auto l_buffer = new char[l_size + 1];
            l_buffer[l_size] = '\0';
            asio::read(*m_socket, asio::buffer(l_buffer, l_size));

            return std::make_shared<Message>(l_buffer);
        }

        int ASIOClient::getSocketID() const
        {
            LOG_DEBUG << m_socket->remote_endpoint().address().to_string();
            return 0;
        }

        void ASIOClient::close()
        {
            m_socket->close();
        }
    }
}