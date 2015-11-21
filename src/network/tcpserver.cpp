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

#include "tcpserver.h"
#include "../tasking/handshake.h"
#include "../log/logger.h"
#include "../common/configuration.h"

namespace jimdb
{
    namespace network
    {
        TCPServer::TCPServer(tasking::TaskQueue& q) : m_listensocket(INVALID_SOCKET) {}

        TCPServer::~TCPServer()
        {
            //close the underlaying socket of this class
            closesocket(m_listensocket);
            //cleanup everything of the sockets
            WSACleanup();
        }

        bool TCPServer::start()
        {
            WSADATA wsaData;
            int iResult;
            // Initialize Winsock
            iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (iResult != 0)
            {
                LOG_ERROR << "WSAStartup failed: " << iResult;
                return false;
            }

            struct addrinfo* result = nullptr, *ptr = nullptr, hints;
            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET ;
            hints.ai_socktype = SOCK_STREAM ;
            hints.ai_protocol = IPPROTO_TCP;
            hints.ai_flags = AI_PASSIVE ;
            // Resolve the local address and port to be used by the server
            iResult = getaddrinfo(nullptr, common::Configuration::getInstance()[common::PORT].GetString(), &hints, &result);
            if (iResult != 0)
            {
                LOG_ERROR << "getaddrinfo failed: " << iResult;
                return false;
            }

            // Create a SOCKET for the server to listen for client connections
            //create socket to listen for clients
            m_listensocket = INVALID_SOCKET;
            m_listensocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

            if (m_listensocket == INVALID_SOCKET)
            {
                LOG_ERROR << "Error at listensock socket(): " << WSAGetLastError();
                freeaddrinfo(result);
                return false;
            }

            // Setup the TCP listening socket
            iResult = bind(m_listensocket, result->ai_addr,
                           static_cast<int>(result->ai_addrlen));
            if (iResult == SOCKET_ERROR)
            {
                LOG_ERROR << "bind failed with error: " << WSAGetLastError();
                freeaddrinfo(result);
                closesocket(m_listensocket);
                return false;
            }

            //free the result
            freeaddrinfo(result);

            //start listening
            if (listen(m_listensocket, SOMAXCONN) == SOCKET_ERROR)
            {
                LOG_ERROR << "Listen failed with error: " << WSAGetLastError();
                closesocket(m_listensocket);
                return false;
            }

            //clear master set
            FD_ZERO(&m_set);
            //add the listen socket to the set
            FD_SET(m_listensocket, &m_set);

            LOG_INFO << "Server started on port: " << common::Configuration::getInstance()[common::PORT].GetString();
            return true;
        }

        void* get_in_addr(struct sockaddr* sa)
        {
            if (sa->sa_family == AF_INET)
            {
                return &(reinterpret_cast<struct sockaddr_in*>(sa)->sin_addr);
            }
            return &(reinterpret_cast<struct sockaddr_in6*>(sa)->sin6_addr);
        }

        int TCPServer::accept(const bool& blocking)
        {
            //check if set in readable set or if the list is empty have a blocking connection
            if (!blocking)
            {
                if (!checkConnection())
                    return 0;
            }
            //we can accept a new
            SOCKET ClientSocket;
            ClientSocket = INVALID_SOCKET;
            // Accept a client socket
            struct sockaddr their_addr; //stored address
            int sin_size = sizeof their_addr;
            ClientSocket = ::accept(m_listensocket, &their_addr, &sin_size);
            if (ClientSocket == INVALID_SOCKET)
            {
                LOG_ERROR << "accept failed: " << WSAGetLastError();
                closesocket(m_listensocket);
                //do not clean!
                return 1;
            }
            //add new Client handler Task
            auto newCl = std::make_shared<ClientHandle>(ClientSocket, their_addr);

            //start with the handshake
            if(!tasking::TaskQueue::getInstance().push_pack(std::make_shared<tasking::HandshakeTask>(newCl)))
            {
                //TODO send feedback that the queue is full
            }

            //now do whatever you want
            char s[INET6_ADDRSTRLEN];
            inet_ntop(their_addr.sa_family,
                      get_in_addr(&their_addr), s,
                      sizeof s);
            LOG_INFO << "TCPServer: got connection from " << s;
            return true;
        }


        bool TCPServer::checkConnection()
        {
            fd_set temp_set;
            FD_ZERO(&temp_set);
            FD_SET(m_listensocket, &temp_set);
            //setup the timeout to 100ms
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 100;
            if (select(temp_set.fd_count, &temp_set, nullptr, nullptr, &tv) == -1)
            {
                //failed to select
                LOG_ERROR << "fail in select method at accept";
            }
            //check if set in readable set
            if (FD_ISSET(m_listensocket, &temp_set))
                return true;
            return false;
        }
    }
}
