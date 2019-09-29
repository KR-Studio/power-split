#include "Socket.h"
#include "assert.h"

namespace PowerSplitNet
{
	Socket::Socket(IPVersion ipVersion, SocketHandler handler)
		:ipVersion(ipVersion), handler(handler)
	{
		assert(ipVersion == IPVersion::IPv4);
	}

	NetResult Socket::Create()
	{
		assert(ipVersion == IPVersion::IPv4);

		if (handler != INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}

		handler = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (handler == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != NetResult::Net_Success)
		{
			return NetResult::Net_NotYetImplemented;
		}

		return NetResult::Net_Success;
	}

	NetResult Socket::Close()
	{
		// Close the socket to release the resources associated
		// Normally an application calls shutdown() before closesocket 
		// to disables sends or receives on a socket first

		if (handler == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}

		int result = closesocket(handler);

		if (result != 0) // if error occurred while trying to close socket
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}

		handler = INVALID_SOCKET;
		return NetResult::Net_Success;
	}

	NetResult Socket::Bind(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = bind(handler, (sockaddr*)(&addr), sizeof(sockaddr_in));
		if (result != 0)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}

		return NetResult::Net_Success;
	}

	NetResult Socket::Listen(IPEndpoint endpoint, int backlog)
	{
		if (Bind(endpoint) != NetResult::Net_Success)
		{
			return NetResult::Net_NotYetImplemented;
		}

		int result = listen(handler, backlog); // backlog - the number of possible connections
		if (result != 0)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}
		return NetResult();
	}

	NetResult Socket::Accept(Socket& outsocket)
	{
		SocketHandler acceptedConnectionHandler = accept(handler, nullptr, nullptr);
		if (acceptedConnectionHandler == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}

		outsocket = Socket(IPVersion::IPv4, acceptedConnectionHandler);
		return NetResult::Net_Success;
	}

	NetResult Socket::Connect(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = connect(handler, (sockaddr*)(&addr), sizeof(sockaddr_in));
		if (result != 0)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}
		return NetResult::Net_Success;
	}

	SocketHandler Socket::GetHandler()
	{
		return handler;
	}

	IPVersion Socket::GetIPVersion()
	{
		return ipVersion;
	}

	NetResult Socket::SetSocketOption(SocketOption option, BOOL value)
	{
		int result = 0;
		switch (option)
		{
			case SocketOption::TCP_NoDelay:
			{
				result = setsockopt(handler, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
				break;
			}
			default:
			{
				return NetResult::Net_NotYetImplemented;
			}
		}

		if (result != 0)
		{
			int error = WSAGetLastError();
			return NetResult::Net_NotYetImplemented;
		}

		return NetResult::Net_Success;
	}
}