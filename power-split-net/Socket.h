#pragma once

#include "IPEndpoint.h"
#include "IPVersion.h"
#include "NetResult.h"
#include "SocketOption.h"
#include "SocketHandler.h"

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
		NetResult Connect(IPEndpoint endpoint);
		SocketHandler GetHandler();
		IPVersion GetIPVersion();
	private:
		NetResult SetSocketOption(SocketOption option, BOOL value);
		IPVersion ipVersion = IPVersion::IPv4;
		SocketHandler handler = INVALID_SOCKET;
	};
}