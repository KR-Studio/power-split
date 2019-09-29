//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#define WIN32_LEAN_AND_MEAN

#include "pch.h"
#include "MainPage.xaml.h"
#include "string"

#include <power-split-net/Including.h>

//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//
//// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "19860"

using namespace PowerSplitClient;

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

using namespace PowerSplitNet;

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

std::wstring i2ws(const int& dataInt)
{
	std::wstring dataWstr = std::to_wstring(dataInt);
	return dataWstr;
}

String^ i2ps(const int& dataInt)
{
	String^ dataPstr = ref new String(i2ws(dataInt).c_str());
	return dataPstr;
}

//WSADATA wsaData;
//SOCKET ConnectSocket = INVALID_SOCKET;
//struct addrinfo* result = NULL, * ptr = NULL, hints;
//char* sendbuf = "this is a test";
//char recvbuf[DEFAULT_BUFLEN];
//int iResult;
//int recvbuflen = DEFAULT_BUFLEN;

void PowerSplitClient::MainPage::PageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Clearing server output area
	textBlockInfoOutput->Text = "";

	////// Validate the parameters
	////if (argc != 2) {
	////	printf("usage: %s server-name\n", argv[0]);
	////}

	//// Initialize Winsock
	//iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//if (iResult != 0) {
	//	std::string iResultStr = "WSAStartup failed with error: " + std::to_string(iResult);
	//	textBlockInfoOutput->Text = s2ps(iResultStr);
	//}
	//else {
	//	std::string iResultStr = "WSAStartup return: " + std::to_string(iResult);
	//	textBlockInfoOutput->Text = s2ps(iResultStr);

	//	ZeroMemory(&hints, sizeof(hints));
	//	hints.ai_family = AF_UNSPEC;
	//	hints.ai_socktype = SOCK_STREAM;
	//	hints.ai_protocol = IPPROTO_TCP;

	//	// Resolve the server address and port

	//	String^ hostPStr = hostTextBox->Text;
	//	String^ portPStr = portTextBox->Text;
	//	std::string host = ps2s(hostPStr);
	//	std::string port = ps2s(portPStr);

	//	iResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	//	if (iResult != 0) {
	//		std::string iResultStr = "\r\nGetaddrinfo failed with error: " + std::to_string(iResult);
	//		textBlockInfoOutput->Text += s2ps(iResultStr);
	//		WSACleanup();
	//	}
	//	else {
	//		std::string iResultStr = "\r\nGetaddrinfo return: " + std::to_string(iResult);
	//		textBlockInfoOutput->Text += s2ps(iResultStr);
	//	}
	//}

	if (Network::Initialize()) {
		std::string dataStr = "WinSock API successfully initialized\r\n";
		textBlockInfoOutput->Text += s2ps(dataStr);
	}
	else {
		std::string dataStr = "WSAStartup failed with error\r\n";
		textBlockInfoOutput->Text += s2ps(dataStr);
	}
}


void PowerSplitClient::MainPage::ConnectBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//// Attempt to connect to an address until one succeeds
	//for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

	//	// Create a SOCKET for connecting to server
	//	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	//	if (ConnectSocket == INVALID_SOCKET) {
	//		std::string WSAGetLastErrorStr = "\r\nSocket failed with error: " + std::to_string(WSAGetLastError());
	//		textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
	//		WSACleanup();
	//	}
	//	else {
	//		std::string WSAGetLastErrorStr = "\r\nSocket return: " + std::to_string(WSAGetLastError());
	//		textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);

	//		// Connect to server.
	//		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	//		if (iResult == SOCKET_ERROR) {
	//			closesocket(ConnectSocket);
	//			ConnectSocket = INVALID_SOCKET;
	//			continue;
	//		}
	//		else {
	//			freeaddrinfo(result);

	//			if (ConnectSocket == INVALID_SOCKET) {
	//				textBlockInfoOutput->Text += "\r\nUnable to connect to server!";
	//				WSACleanup();
	//			}
	//			else {
	//				textBlockInfoOutput->Text += "\r\nСonnection to server successful!";

	//				// Send an initial buffer
	//				iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	//				if (iResult == SOCKET_ERROR) {
	//					std::string WSAGetLastErrorStr = "\r\nSend failed with error: " + std::to_string(WSAGetLastError());
	//					textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
	//					closesocket(ConnectSocket);
	//					WSACleanup();
	//				}
	//				else {
	//					std::string iResultStr = "\r\nBytes sent: " + std::to_string(iResult);
	//					textBlockInfoOutput->Text += s2ps(iResultStr);
	//				}
	//			}
	//		}
	//		break;
	//	}
	//}
}


void PowerSplitClient::MainPage::DisconenctBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//// shutdown the connection since no more data will be sent
	//iResult = shutdown(ConnectSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR) {
	//	std::string WSAGetLastErrorStr = "\r\nShutdown failed with error: " + std::to_string(WSAGetLastError());
	//	textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//}
	//else {
	//	std::string WSAGetLastErrorStr = "\r\nShutdown return: " + std::to_string(WSAGetLastError());
	//	textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);

	//	// Receive until the peer closes the connection
	//	do {
	//		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	//		if (iResult > 0) {
	//			std::string iResultStr = "\r\nBytes received: " + std::to_string(iResult);
	//			textBlockInfoOutput->Text += s2ps(iResultStr);
	//		}
	//		else if (iResult == 0) {
	//			textBlockInfoOutput->Text += "\r\nConnection closed" + "\n";

	//			// cleanup
	//			closesocket(ConnectSocket);
	//			WSACleanup();
	//		}
	//		else {
	//			std::string WSAGetLastErrorStr = "\r\nRecv failed with error: " + std::to_string(WSAGetLastError()) + "\n";
	//			textBlockInfoOutput->Text += s2ps(WSAGetLastErrorStr);

	//			// cleanup
	//			closesocket(ConnectSocket);
	//			WSACleanup();
	//		}
	//	} while (iResult > 0);
	//}
}


void PowerSplitClient::MainPage::SubmitBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
