/**
############################################################################
# GPL License                                                              #
#                                                                          #
# This file is part of the JIM - DB.                                         #
# Copyright(c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
# This program is free software : you can redistribute it and / or modify     #
# it under the terms of the GNU General Public License as                  #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful, #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with this program.If not, see <http://www.gnu.org/licenses/>.     #
############################################################################
**/

#pragma once
namespace jimdb
{
    namespace network
    {

        template <typename AllowTime>
        void ASIOClienthandle::await_operation(AllowTime const& deadline_or_duration)
        {
            using namespace asio;

            auto& ioservice = m_socket->get_io_service();
            ioservice.reset();
            {
                asio::steady_timer tm(ioservice, deadline_or_duration);
                tm.async_wait([this](std::error_code ec)
                {
                    if (ec != error::operation_aborted) m_socket->cancel();
                });
                ioservice.run_one();
            }
            ioservice.run();
        }
    }
}