// power-split-6-server-cli.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <iostream>
#include <sstream>
#include <math.h>  
#include <cmath>
#include <power-split-net/Including.h>
#include <list>
#include <iterator>

using namespace PowerSplitNet;

// Check number of messages
int numberOfMessage = 0;

int main()
{
	if (Network::Initialize()) {
		std::string outputDataStr = "WinSock API successfully initialized";
		std::cout << outputDataStr << std::endl;

		NetSocket socketListener;
		if (socketListener.Create() == NetResult::Net_Success)
		{
			std::string outputDataStr = "Socket successfully created";
			std::cout << outputDataStr << std::endl;

			if (socketListener.Listen(IPEndpoint("0.0.0.0", 80)) == NetResult::Net_Success) // 0.0.0.0 means that access can be obtained form any IP
			{
				std::string outputDataStr = "Socket successfully listening on port 80";
				std::cout << outputDataStr << std::endl;

				NetSocket socketConnected;
				if (socketListener.Accept(socketConnected) == NetResult::Net_Success)
				{
					std::string outputDataStr = "New connection accepted";
					std::cout << outputDataStr << std::endl;

					uint32_t messageNumberReceived, messageSizeReceived;
					std::string messageDataReceived;
					NetPacket packetReceived;

					uint32_t messageNumberToSend, messageSizeToSend;
					std::string messageDataToSend;
					NetPacket packetToSend;
					
					NetResult resultReceived = NetResult::Net_Success;
					while (resultReceived == NetResult::Net_Success)
					{
						resultReceived = socketConnected.Receive(packetReceived);
						if (resultReceived != NetResult::Net_Success)
							break;
						else
						{
							std::cout << "Data received: ";
							try
							{
								packetReceived >> messageNumberReceived >> messageSizeReceived;
							}
							catch (NetPacketException& exception)
							{
								std::cout << exception.CStr() << std::endl;
							}

							if (messageNumberReceived <= 1) {
								try
								{
									packetReceived >> messageDataReceived;
								}
								catch (NetPacketException& exception)
								{
									std::cout << exception.CStr() << std::endl;
								}

								std::string outputDataStr = messageDataReceived;
								std::cout << outputDataStr << std::endl;

								messageDataToSend = "Hello, Client!";
								messageNumberToSend = ++numberOfMessage;
								messageSizeToSend = messageDataToSend.size();
								packetToSend << messageNumberToSend << messageSizeToSend << messageDataToSend;

							}
							else {
								try
								{
									packetReceived >> messageDataReceived;
								}
								catch (NetPacketException& exception)
								{
									std::cout << exception.CStr() << std::endl;
								}
							}

							NetResult resultSent = NetResult::Net_Success;
							while (resultSent == NetResult::Net_Success)
							{
								std::string outputDataStr = "Attempting to send set of data...";
								std::cout << outputDataStr << std::endl;

								resultSent = socketConnected.Send(packetToSend);
								if (resultSent != NetResult::Net_Success)
									break;

								outputDataStr = "Data has been sent to client";
								std::cout << outputDataStr << std::endl;
								break;
							}
						}
					}

					/*socketConnected.Close();
					outputDataStr = "NewConnectionSocket successfully closed";
					std::cout << outputDataStr << std::endl;*/
				}
				else
				{
					std::string outputDataStr = "Failed to accept new connection";
					std::cout << outputDataStr << std::endl;
				}
			}
			else
			{
				std::string outputDataStr = "Failed to listen socket on port 80";
				std::cout << outputDataStr << std::endl;
			}

			socketListener.Close();
			outputDataStr = "Socket successfully closed";
			std::cout << outputDataStr << std::endl;
		}
		else
		{
			std::string outputDataStr = "Socket failed to create";
			std::cout << outputDataStr << std::endl;
		}
	}
	else {
		std::string outputDataStr = "WSAStartup failed with error";
		std::cout << outputDataStr << std::endl;
	}

	Network::Shutdown();
	std::string outputDataStr = "WinSock API successfully closed";
	std::cout << outputDataStr << std::endl;

	system("pause");
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
