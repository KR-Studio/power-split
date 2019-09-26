//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "19860"

using namespace PowerSplitServer;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409



MainPage::MainPage()
{
	InitializeComponent();
}

std::wstring s2ws(const std::string& dataStr)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &dataStr[0], (int)dataStr.size(), NULL, 0);
	std::wstring dataWstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &dataStr[0], (int)dataStr.size(), &dataWstr[0], size_needed);
	return dataWstr;
}

String^ s2ps(const std::string& dataStr) 
{
	std::wstring dataWstr = s2ws(dataStr);
	String^ dataPstr = ref new String(dataWstr.c_str());
	return dataPstr;
}

std::string ws2s(const std::wstring& dataWstr)
{
	auto dataWide = dataWstr.c_str();
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, dataWide, -1, nullptr, 0, NULL, NULL);
	auto dataUtf8 = std::make_unique<char[]>(bufferSize);
	if (0 == WideCharToMultiByte(CP_UTF8, 0, dataWide, -1, dataUtf8.get(), bufferSize, NULL, NULL))
		throw std::exception("Can't convert string to UTF8");
	return std::string(dataUtf8.get());
}

std::string ps2s(String^& dataPstr)
{
	std::wstring dataPstrWstr(dataPstr->Data());
	auto dataStr = ws2s(dataPstrWstr);
	return dataStr;
}

WSADATA wsaData;
int iResult;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

struct addrinfo* result = NULL;
struct addrinfo hints;

int iSendResult;
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

void PowerSplitServer::MainPage::PageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::string iResultStr = "WSAStartup failed with error: " + std::to_string(iResult);
		textBlockInfoOutput->Text = s2ps(iResultStr);
	}
	else {
		std::string iResultStr = "WSAStartup return: " + std::to_string(iResult);
		textBlockInfoOutput->Text = s2ps(iResultStr);

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			std::string iResultStr = "\r\nGetaddrinfo failed with error: " + std::to_string(iResult);
			textBlockInfoOutput->Text += s2ps(iResultStr);
			WSACleanup();
		}
		else {
			std::string iResultStr = "\r\nGetaddrinfo return: " + std::to_string(iResult);
			textBlockInfoOutput->Text += s2ps(iResultStr);
		}
	}
}


void PowerSplitServer::MainPage::StartButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::string WSAGetLastErrorStr = "\r\nSocket failed with error: " + std::to_string(WSAGetLastError());
		textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
		freeaddrinfo(result);
		WSACleanup();
	}
	else {
		std::string WSAGetLastErrorStr = "\r\nSocket return: " + std::to_string(WSAGetLastError());
		textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			std::string WSAGetLastErrorStr = "\r\nBind failed with error: " + std::to_string(WSAGetLastError());
			textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
		}
		else {
			std::string WSAGetLastErrorStr = "\r\nBind return: " + std::to_string(WSAGetLastError());
			textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);

			freeaddrinfo(result);

			iResult = listen(ListenSocket, SOMAXCONN);
			if (iResult == SOCKET_ERROR) {
				std::string WSAGetLastErrorStr = "\r\nListen failed with error: " + std::to_string(WSAGetLastError());
				textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
				closesocket(ListenSocket);
				WSACleanup();
			}
			else {
				std::string WSAGetLastErrorStr = "\r\nListen return: " + std::to_string(WSAGetLastError());
				textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);

				// Accept a client socket
				ClientSocket = accept(ListenSocket, NULL, NULL);
				if (ClientSocket == INVALID_SOCKET) {
					std::string WSAGetLastErrorStr = "\r\nAccept failed with error: " + std::to_string(WSAGetLastError());
					textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
					closesocket(ListenSocket);
					WSACleanup();
				}
				else {
					std::string WSAGetLastErrorStr = "\r\nAccept return: " + std::to_string(WSAGetLastError());
					textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);

					// No longer need server socket
					closesocket(ListenSocket);

					// Receive until the peer shuts down the connection
					do {
						iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
						if (iResult > 0) {
							std::string iResultStr = "\r\nBytes received: " + std::to_string(iResult);
							textBlockInfoOutput->Text += s2ps(iResultStr);

							// Echo the buffer back to the sender
							iSendResult = send(ClientSocket, recvbuf, iResult, 0);
							if (iSendResult == SOCKET_ERROR) {

								std::string WSAGetLastErrorStr = "\r\nSend failed with error: " + std::to_string(WSAGetLastError());
								textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
								closesocket(ClientSocket);
								WSACleanup();
							}
							else {
								std::string iSendResultStr = "\r\nBytes sent: " + std::to_string(iSendResult);
								textBlockInfoOutput->Text += s2ps(iSendResultStr);
							}
						}
						else if (iResult == 0) {
							textBlockInfoOutput->Text += "\r\nConnection closing...";
							closesocket(ClientSocket);
							WSACleanup();
						}
						else {
							std::string WSAGetLastErrorStr = "\r\nRecv failed with error: " + std::to_string(WSAGetLastError());
							textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
							closesocket(ClientSocket);
							WSACleanup();
						}
					} while (iResult > 0);
				}
			}
		}
	}
}


void PowerSplitServer::MainPage::CloseButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (ListenSocket != INVALID_SOCKET) {
		// shutdown the connection since we're done
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::string WSAGetLastErrorStr = "\r\nShutdown failed with error: " + std::to_string(WSAGetLastError()) + "\n";
			textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
			closesocket(ClientSocket);
			WSACleanup();
		}
		else {
			std::string WSAGetLastErrorStr = "\r\nShutdown return: " + std::to_string(WSAGetLastError()) + "\n";
			textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
		}

		// cleanup
		closesocket(ClientSocket);
		WSACleanup();
	}
	else {
		textBlockInfoOutput->Text += "\r\nThere is no active socket in that moment..";
	}
}
