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

double add(double op1, double op2) {
	double sum = op1 + op2;
	return sum;
}

double subtract(double op1, double op2) {
	double diff = op1 - op2;
	return diff;
}

double multiply(double op1, double op2) {
	double product = op1 * op2;
	return product;
}

double divide(double op1, double op2) {
	double result = op1 / op2;
	return result;
}

double sine(double op) {
	double result = sin(op);
	return result;
}

double tang(double op) {
	double result = tan(op);
	return result;
}

double cosine(double op) {
	double result = cos(op);
	return result;
}

double cotang(double op) {
	double result = 1 / tan(op);
	return result;
}

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

		NetSocket socket;
		if (socket.Create() == NetResult::Net_Success)
		{
			std::string dataStr = "Socket successfully created";
			std::cout << dataStr << std::endl;

			if (socket.Listen(IPEndpoint("0.0.0.0", 80)) == NetResult::Net_Success) // 0.0.0.0 means that access can be obtained form any IP
			{
				std::string dataStr = "Socket successfully listening on port 80";
				std::cout << dataStr << std::endl;

				NetSocket newConnection;	
				if (socket.Accept(newConnection) == NetResult::Net_Success)
				{
					std::string dataStr = "New connection accepted";
					std::cout << dataStr << std::endl;

					std::string buffer = "";
					NetResult result = NetResult::Net_Success;
					while (result == NetResult::Net_Success)
					{
						uint32_t bufferSize = buffer.size();
						result = newConnection.ReceiveAll(&bufferSize, sizeof(uint32_t));
						if (result != NetResult::Net_Success)
							break;
						else
						{
							bufferSize = ntohl(bufferSize); // network to host by long

							if (bufferSize > PowerSplitNet::MAX_PACKETSIZE)
								break;

							buffer.resize(bufferSize);
							result = newConnection.ReceiveAll(&buffer[0], bufferSize);
							if (result != NetResult::Net_Success)
								break;
							else
							{
								std::cout << "Data received: ";
								std::string dataStr = '[' + std::to_string(bufferSize) + ']' + ' ' + buffer;
								std::cout << dataStr << std::endl;

								std::vector<std::string> dataWordsStr;
								std::istringstream ist(dataStr);
								std::string tmp;
								while (ist >> tmp)
									dataWordsStr.emplace_back(tmp);

								//std::cout << "Words:" << std::endl;
								//for (int i = 3; i < dataWordsStr.size(); ++i)
								//	std::cout << dataWordsStr[i] << ' ';
								
								//break;

								std::cout << "Result successfully sent to client.";


								NetResult result = NetResult::Net_Success;
								while (result == NetResult::Net_Success)
								{
									uint32_t bufferSize = buffer.size();
									bufferSize = htonl(bufferSize); // host to network by long
									result = socket.SendAll(&bufferSize, sizeof(uint32_t));
									if (result != NetResult::Net_Success)
										break;
									else
									{
										result = socket.SendAll(buffer.data(), buffer.size());
										if (result != NetResult::Net_Success)
											break;
										else
										{
											std::string dataStr = "Attempting to send set of data...";
												std::cout << dataStr << std::endl;
											break;
										}
									}
								}
							}
						}
					}

					/*newConnection.Close();
					dataStr = "NewConnectionSocket successfully closed";
					std::cout << dataStr << std::endl;*/
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