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

#include "asioclienthandle.h"
#define MESSAGE_SIZE 8

namespace jimdb
{
    namespace network
    {
        ASIOClienthandle::ASIOClienthandle(std::shared_ptr<asio::ip::tcp::socket> socket) : m_socket(socket) {}

        ASIOClienthandle::~ASIOClienthandle()
        {
            m_socket->close();// ? needed?
        }

        bool ASIOClienthandle::send(std::shared_ptr<std::string> s)
        {
            char length[MESSAGE_SIZE + 1];
            sprintf(length, "%8d", static_cast<int>(s->size()));
            auto l_message = std::string(length);
            l_message += *s;
            asio::write(*m_socket, asio::buffer(l_message), asio::transfer_all());
            return true;
        }

        bool ASIOClienthandle::isConnected() const
        {
            return true;
        }

        std::shared_ptr<Message> ASIOClienthandle::getData()
        {
            char size[MESSAGE_SIZE + 1];
            asio::async_read(*m_socket, asio::buffer(size, MESSAGE_SIZE), [&](std::error_code ec, size_t bytes_read)
            {
                if (ec) throw std::runtime_error(ec.message());
            });
            //wait for the task to finish
            await_operation(std::chrono::seconds(1));

            auto l_size = atoi(size);
            auto l_buffer = new char[l_size + 1];
            l_buffer[l_size] = '\0';
            asio::async_read(*m_socket, asio::buffer(l_buffer, l_size), [&](std::error_code ec, size_t bytes_read)
            {
                if (ec) throw std::runtime_error(ec.message());
            });
            //wait for the task to finish
            await_operation(std::chrono::seconds(1));

            return std::make_shared<Message>(l_buffer);
        }
    }
}