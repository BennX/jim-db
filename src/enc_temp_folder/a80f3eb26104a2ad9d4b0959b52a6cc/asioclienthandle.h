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
#include "iclient.h"

#define ASIO_STANDALONE
#define ASIO_HAS_STD_CHRONO
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_ADDRESSOF
#include <asio.hpp>
namespace jimdb
{
    namespace network
    {
        class ASIOClienthandle : public IClient
        {
        public:
            explicit ASIOClienthandle(std::shared_ptr<asio::ip::tcp::socket> socket);
            ~ASIOClienthandle();
            /**
            \brief sending data to the client

            @param[in] string the message to send
            @author Benjamin Meyer
            @date 21.11.2015 18:28
            */
            bool send(std::shared_ptr<std::string> s) override;
            /**
            \brief get Data with a async call
            It does get data async for 1 second or throws!

            @throw error if nothing got recv or timeout
            @return a Message object of the data that got received
            @author Benjamin Meyer
            @date 21.11.2015 18:26
            */
            std::shared_ptr<Message> getData() override;

        private:
            std::shared_ptr<asio::ip::tcp::socket> m_socket;
            //@return if it was canceld
            template<typename AllowTime> void await_operation(AllowTime const& deadline_or_duration);
			volatile bool m_cancled;

			char* read(const size_t& count);

		};
    }
}
#include "asioclienthandle.hpp"