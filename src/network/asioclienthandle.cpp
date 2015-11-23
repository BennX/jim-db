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
        ASIOClienthandle::ASIOClienthandle(std::shared_ptr<asio::ip::tcp::socket> socket) : m_socket(socket),
            m_cancled(false) {}

        ASIOClienthandle::~ASIOClienthandle()
        {
            m_socket->close();
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
                              size_t bytes_read)
            {
                //if (ec)
                //    return false;
            });
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

            l_buffer = read(l_size);
            if(l_buffer != nullptr)
                return std::make_shared<Message>(l_buffer);
            return nullptr;
        }

        char* ASIOClienthandle::read(const size_t& count)
        {
            auto l_buffer = new char[count + 1];
            l_buffer[count] = '\0';

            asio::async_read(*m_socket, asio::buffer(l_buffer, count), [&](std::error_code ec, size_t bytes_read)
            {
                if (!ec)
                {

                    // hier muss der kram ab std::stringstream hin, denn hier läuft es synchron (innerhalb der asynchronen struktur)
                    // alternativ geht dann auch hier auch ein Abbruch, wenn alles fertig gelesen wurde und dann im Finish-Callback
                    // die Verarbeitung übernehmen
                    // Nebenläufigkeit:
                    // getData
                    // |
                    // |----> async handshake
                    // |            |
                    // wait return  |
                    // |            |-------> async message
                    // |            |             |
                    // |            |             |
                    // finish<------|<-------------
                    //
                    // das Wait muss die Zeit für den Timeout abwarten und dann direkt returnen und eben alle asynchronen Teile
                    // mit beenden, das finish muss so lange warten, bis beide async Läufe beendet wurden
                    //return l_buffer;
                }
            });
            //wait for the task to finish
            if (await_operation(std::chrono::seconds(1)))
            {
                return nullptr;
            }
            return l_buffer;
        }
    }
}