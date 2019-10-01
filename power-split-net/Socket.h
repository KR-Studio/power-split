#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "IPEndpoint.h"
#include "IPVersion.h"
#include "NetResult.h"
#include "SocketOption.h"
#include "SocketHandler.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

namespace PowerSplitNet
{
	class Socket
	{
	public:
		Socket(IPVersion ipVersion = IPVersion::IPv4, SocketHandler handler = INVALID_SOCKET);
		NetResult Create();
		NetResult Close();
		NetResult Bind(IPEndpoint endpoint);
		NetResult Listen(IPEndpoint endpoint, int backlog = 5);
		NetResult Accept(Socket& outsocket);
		NetResult Receive(void * dataDestination, int numberOfBytes, int & bytesRecieved);
		NetResult ReceiveAll(void * dataDestination, int numberOfBytes);
		NetResult Connect(IPEndpoint endpoint);
		NetResult Send(const void * dataSource, int numberOfBytes, int & bytesSent);
		NetResult SendAll(const void * dataSource, int numberOfBytes);
		SocketHandler GetHandler();
		IPVersion GetIPVersion();
	private:
		NetResult SetSocketOption(SocketOption option, BOOL value);
		IPVersion ipVersion = IPVersion::IPv4;
		SocketHandler handler = INVALID_SOCKET;
	};
}