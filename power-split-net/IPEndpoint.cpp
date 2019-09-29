#include "IPEndpoint.h"
#include "assert.h"

namespace PowerSplitNet
{
	IPEndpoint::IPEndpoint(const char* ip, unsigned short port)
	{
		this->port = port;
		in_addr addr; // location to store IPv4 address

		int result = inet_pton(AF_INET, ip, &addr); // presentation format to network format
		if (result == 1)
		{
			if (addr.S_un.S_addr != INADDR_NONE)
			{
				ipStr = ip;
				ipVersion = IPVersion::IPv4;
				ipBytes.resize(sizeof(ULONG));
				memcpy(&ipBytes[0], &addr.S_un.S_addr, sizeof(ULONG));
				hostname = ip;
				return;
			}
		}

		// Attempt to resolve hostname to IPv4 address
		addrinfo hints = {}; // hints will filter the results we get back for getaddrinfo()
		hints.ai_family = AF_INET; // IPv4 addresses only
		addrinfo* hostinfo = nullptr;

		result = getaddrinfo(ip, NULL, &hints, &hostinfo);
		if (result == 0)
		{
			sockaddr_in* hostaddr = reinterpret_cast<sockaddr_in*> (hostinfo->ai_addr);

			//host_addr->sin_addr.S_un.S_addr
			ipStr.resize(16);
			inet_ntop(AF_INET, &hostaddr->sin_addr, &ipStr[0], sizeof(16));

			hostname = ip;

			ULONG ipLong = hostaddr->sin_addr.S_un.S_addr;
			ipBytes.resize(sizeof(ULONG));
			memcpy(&ipBytes[0], &ipLong, sizeof(ULONG));

			ipVersion = IPVersion::IPv4;

			freeaddrinfo(hostinfo); // memory clean up from getaddrinfo()
			return;
		}
	}

	IPVersion IPEndpoint::GetIpVersion()
	{
		return ipVersion;
	}

	std::string IPEndpoint::GetIpStr()
	{
		return ipStr;
	}

	std::vector<uint8_t> IPEndpoint::GetIpBytes()
	{
		return ipBytes;
	}

	std::string IPEndpoint::GetHostname()
	{
		return hostname;
	}

	unsigned short IPEndpoint::GetPort()
	{
		return port;
	}

	sockaddr_in IPEndpoint::GetSockaddrIPv4()
	{
		assert(ipVersion == IPVersion::IPv4);

		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr, &ipBytes[0], sizeof(ULONG));
		addr.sin_port = htons(port); // host to network by short

		return addr;
	}
}