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
        ASIOClienthandle::ASIOClienthandle(std::shared_ptr<asio::ip::tcp::socket> socket) : m_socket(socket), m_cancled(false) {}

        ASIOClienthandle::~ASIOClienthandle()
        {
            m_socket->close();
        }

        bool ASIOClienthandle::send(std::shared_ptr<std::string> s)
        {
            char length[MESSAGE_SIZE + 1];
            sprintf(length, "%8d", static_cast<int>(s->size()));
            auto l_message = std::string(length);
            l_message += *s;
            asio::async_write(*m_socket, asio::buffer(l_message, l_message.size()), [&](std::error_code ec, size_t bytes_read)
            {
                //if (ec) throw std::runtime_error(ec.message());
            });
            return !await_operation(std::chrono::seconds(1));
        }

        std::shared_ptr<Message> ASIOClienthandle::getData()
        {
            char size[MESSAGE_SIZE + 1];
            asio::async_read(*m_socket, asio::buffer(size, MESSAGE_SIZE), [&](std::error_code ec, size_t bytes_read)
            {
                if (ec)
                    return;
                
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
                
            });
            //wait for the task to finish
            if(await_operation(std::chrono::seconds(1)))
            {
                return nullptr;
            }
            // hier ist Ende der Methode, d.h. hier wird entweder beim Cancel der Nullpointer geliefert oder eben die Daten

            std::stringstream ss;
            ss << size;
            int l_size;
            ss >> l_size;
            //auto l_size = atoi(size);
            auto l_buffer = new char[l_size + 1];
            l_buffer[l_size] = '\0';
            asio::async_read(*m_socket, asio::buffer(l_buffer, l_size), [&](std::error_code ec, size_t bytes_read)
            {
                if (ec)
                    return nullptr;
            });
            //wait for the task to finish
            if(await_operation(std::chrono::seconds(1)))
            {
                return nullptr;
            }

            return std::make_shared<Message>(l_buffer);
        }
    }
}