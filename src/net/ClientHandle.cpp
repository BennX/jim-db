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

#include "ClientHandle.h"
#include "../log/Logger.h"


ClientHandle::ClientHandle(const SOCKET& s, const sockaddr_storage& add) : m_sock(s), m_addr(add),m_connected(true)
{
	struct timeval clientTv;
	clientTv.tv_sec = 0;
	clientTv.tv_usec = 100;
	setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&clientTv), sizeof(struct timeval));
	handShake();
}

ClientHandle::~ClientHandle()
{
	closesocket(m_sock);
}

bool ClientHandle::operator<<(std::shared_ptr<std::string> s)
{
	return send(s);
}

bool ClientHandle::send(std::shared_ptr<std::string> s)
{
	if (!m_connected)
		return false;

	char length[sizeof(int) + 1];
	sprintf(length, "%4d", static_cast<int>(s->size()));
	auto toSend = std::string(length);
	toSend += *s; //add the message

	int iSendResult;
	iSendResult = ::send(m_sock, toSend.c_str(), toSend.size(), 0);
	if (iSendResult == SOCKET_ERROR)
	{
		LOG_ERROR << "send failed: " << WSAGetLastError() << ": Closing the sock";
		m_connected = false;
		closesocket(m_sock);
		return false;
	}
	LOG_INFO << "Sent: " << *s;
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

bool ClientHandle::getData(std::shared_ptr<std::string> ptr)
{
	int n;
	ptr->clear();

	char size[4];
	n = recv(m_sock, size, sizeof(size), 0);

	//check if retval passed
	if (!checkRetValRecv(n))
		return false;

	//calc how much data
	auto msgLen = atoi(size);
	LOG_INFO << "recv a message with size: " << msgLen;

	//create buffer for the data
	auto buffer = new char[msgLen];

	//read buffer
	n = recv(m_sock, buffer, msgLen, 0);


	//check retval if it passes
	if (!checkRetValRecv(n))
		return false;
	ptr->append(buffer, msgLen);
	LOG_INFO << "Message: " << *ptr;

	delete[] buffer; 
	return true;
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
		LOG_INFO << "Client disconnected.";
		m_connected = false;
		return false;
	}
	return true;
}

bool ClientHandle::handShake()
{
	int r = rand(); //add a random value to the handshake
	auto handshake = std::make_shared<std::string>("HI");
	*handshake += r;
	//sending a handshake HI and wait 1s to return a hi as shake
	send(handshake);
	if (!hasData())
	{
		LOG_INFO << "handshake Failed";
		m_connected = false;
		return false;
	}
	else
	{
		auto string = std::make_shared<std::string>();
		getData(string);

		if (*handshake == *string)
			LOG_INFO << "Handshake successful";
		else
		{
			LOG_INFO << "handshake Failed";
			m_connected = false;
			return false;
		}
	}
	return true;
}