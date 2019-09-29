#include <string>
#include <iostream>

#include <power-split-net/Including.h>

using namespace PowerSplitNet;

int main()
{
	if (Network::Initialize()) {
		std::string dataStr = "WinSock API successfully initialized";
		std::cout << dataStr << std::endl;

		//IPEndpoint ip("www.google.com", 8080);
		//if (ip.GetIpVersion() == IPVersion::IPv4)
		//{
		//	std::string dataStr = "Hostname: " + ip.GetHostname() + "\r\n";
		//	textBlockInfoOutput->Text += s2ps(dataStr);
		//	dataStr = "IP: " + ip.GetIpStr() + "\r\n";
		//	textBlockInfoOutput->Text += s2ps(dataStr);
		//	dataStr = "Port: " + std::to_string(ip.GetPort()) + "\r\n";
		//	textBlockInfoOutput->Text += s2ps(dataStr);
		//	dataStr = "Bytes... ";
		//	for (auto& digit : ip.GetIpBytes())
		//	{
		//		dataStr += std::to_string((int)digit) + " ";
		//	}
		//	dataStr += "\r\n";
		//	textBlockInfoOutput->Text += s2ps(dataStr);
		//}
		//else
		//{
		//	std::string dataStr = "Error occurs when trying to get access to non-IPv4 address\r\n";
		//	textBlockInfoOutput->Text += s2ps(dataStr);
		//}

		Socket socket;
		if (socket.Create() == NetResult::Net_Success)
		{
			std::string dataStr = "Socket successfully created";
			std::cout << dataStr << std::endl;

			if (socket.Listen(IPEndpoint("0.0.0.0", 80)) == NetResult::Net_Success) // 0.0.0.0 means that access can be obtained form any IP
			{
				std::string dataStr = "Socket successfully listening on port 80";
				std::cout << dataStr << std::endl;

				Socket newConnection;
				if (socket.Accept(newConnection) == NetResult::Net_Success)
				{
					std::string dataStr = "New connection accepted";
					std::cout << dataStr << std::endl;

					newConnection.Close();
					dataStr = "NewConnectionSocket successfully closed";
					std::cout << dataStr << std::endl;
				}
				else
				{
					std::string dataStr = "Failed to accept new connection";
					std::cout << dataStr << std::endl;
				}
			}
			else
			{
				std::string dataStr = "Failed to listen socket on port 80";
				std::cout << dataStr << std::endl;
			}

			socket.Close();
			dataStr = "Socket successfully closed";
			std::cout << dataStr << std::endl;
		}
		else
		{
			std::string dataStr = "Socket failed to create";
			std::cout << dataStr << std::endl;
		}
	}
	else {
		std::string dataStr = "WSAStartup failed with error";
		std::cout << dataStr << std::endl;
	}

	Network::Shutdown();
	std::string dataStr = "WinSock API successfully closed";
	std::cout << dataStr << std::endl;

	system("pause");
	return 0;
}