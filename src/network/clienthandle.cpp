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

#include "clienthandle.h"
#include "../log/logger.h"
#include "message.h"

#define MESSAGE_SIZE 8
namespace jimdb
{
    namespace network
    {
        ClientHandle::ClientHandle(const SOCKET& s, const sockaddr& add) : m_sock(s), m_addr(add), m_connected(true)
        {
            //struct timeval clientTv;
            //clientTv.tv_sec = 0;
            //clientTv.tv_usec = 100;
            //setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&clientTv), sizeof(struct timeval));
        }

        ClientHandle::~ClientHandle()
        {
            closesocket(m_sock);
            LOG_DEBUG << "Client:" << m_sock << " closed";
        }

        bool ClientHandle::operator<<(std::shared_ptr<std::string> s)
        {
            return send(s);
        }

        bool ClientHandle::send(std::shared_ptr<std::string> s)
        {
            if (!m_connected)
                return false;
            char length[MESSAGE_SIZE + 1];
            sprintf(length, "%8d", static_cast<int>(s->size()));
            auto toSend = std::string(length);
            toSend += *s; //add the message
            int iSendResult;
            iSendResult = ::send(m_sock, toSend.c_str(), static_cast<int>(toSend.length()), 0);
            if (iSendResult == SOCKET_ERROR)
            {
                LOG_ERROR << "send failed: " << WSAGetLastError() << ": Closing the sock";
                m_connected = false;
                closesocket(m_sock);
                return false;
            }
            //LOG_DEBUG << "Sent: " << *s;
            return true;
        }

        bool ClientHandle::hasData()
        {
            fd_set temp;
            FD_ZERO(&temp);
            FD_SET(m_sock, &temp);
            //setup the timeout to 1000ms
            struct timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            if (select(m_sock + 1, &temp, nullptr, nullptr, &tv) == -1)
            {
                return false;
            }
            if (FD_ISSET(m_sock, &temp))
                return true;
            return false;
        }

        bool ClientHandle::isConnected() const
        {
            return m_connected;
        }

        void ClientHandle::close()
        {
            m_connected = false;
            closesocket(m_sock);
        }

        std::shared_ptr<Message> ClientHandle::getData()
        {
            int n;
            char size[MESSAGE_SIZE + 1];
            n = recv(m_sock, size, sizeof(size) - 1, 0);
            //check if retval passed
            if (!checkRetValRecv(n))
                return nullptr;
            //calc how much data
            auto msgLen = atoi(size);
            //LOG_DEBUG << "recv a message with size: " << msgLen;
            //create buffer for the data
            auto buffer = new char[msgLen + 1]; //one more for nullterm
            buffer[msgLen] = '\0';//add null term
            //read buffer
            n = recv(m_sock, buffer, msgLen, 0);

            //check retval if it passes
            if (!checkRetValRecv(n))
                return nullptr;
            //LOG_DEBUG << "Buffer Message: " << buffer;
            //return ne Message
            return std::make_shared<Message>(buffer);
        }

        int ClientHandle::getSocketID() const
        {
            return m_sock;
        }

        bool ClientHandle::checkRetValRecv(const int& n)
        {
            if (n == SOCKET_ERROR)
            {
                size_t err = WSAGetLastError();
                if (err == WSAECONNRESET)
                {
                    LOG_ERROR << "Failed to recv! Disconnecting from client: " << "Connection reset by peer.";
                }
                else
                {
                    LOG_ERROR << "Disconnecting from client. WSAError: " << err;
                }
                m_connected = false;
                return false;
            }
            if (n == 0)
            {
                LOG_DEBUG << "Client disconnected.";
                m_connected = false;
                return false;
            }
            return true;
        }
    }
}
