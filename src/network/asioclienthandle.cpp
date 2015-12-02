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
#include <thread>

namespace jimdb
{
    namespace network
    {
        unsigned long long ASIOClienthandle::id_counter = 0;

	    unsigned long long ASIOClienthandle::getID() { return m_id; }

	    ASIOClienthandle::ASIOClienthandle(std::shared_ptr<asio::ip::tcp::socket> socket) : m_socket(socket),
            m_cancled(false), m_id(++id_counter) {}

        ASIOClienthandle::~ASIOClienthandle()
        {
			//LOG_INFO << *m_socket;
            m_socket->close();
			//LOG_INFO << *m_socket;
        }

        bool ASIOClienthandle::send(std::shared_ptr<std::string> s)
        {
            if (s == nullptr)
                return false;
            char length[MESSAGE_SIZE + 1];
            sprintf(length, "%8d", static_cast<int>(s->size()));
            auto l_message = std::string(length);
            l_message += *s;

            //this doesnt like string itself getting xstring issues so go for the cstring.
            asio::async_write(*m_socket, asio::buffer(l_message.c_str(), l_message.size()), [&](std::error_code ec,
            size_t bytes_read) {});
            await_operation(std::chrono::seconds(1));
            return true;
        }

        std::shared_ptr<Message> ASIOClienthandle::getData()
        {
            auto l_buffer = read(MESSAGE_SIZE);
            if (l_buffer == nullptr)
                return nullptr;

            //never use atoi
            auto l_size = 0;
            std::stringstream ss;
            ss << l_buffer;
            ss >> l_size;

            delete[] l_buffer;

            if (l_size == 0)
                return nullptr;

            l_buffer = read(l_size);
            if (l_buffer != nullptr)
            {
                return std::make_shared<Message>(l_buffer);
            }
            return nullptr;
        }

        char* ASIOClienthandle::read(const size_t& count)
        {
            auto l_buffer = new char[count + 1];
            l_buffer[count] = '\0';
            asio::async_read(*m_socket, asio::buffer(l_buffer, count), [&](std::error_code ec, size_t bytes_read)
            {
                if (ec)
                    LOG_ERROR << ec.message();
            });
            //wait for the task to finish
            await_operation(std::chrono::seconds(1));
            return l_buffer;
        }
    }
}