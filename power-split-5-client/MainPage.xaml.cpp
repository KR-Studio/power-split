﻿//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "string"
#include <sstream>

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


// Vector with methods that have activeState in checkBoxes
std::vector<std::string> checkBoxesActive = { };
// Main client Socket
NetSocket socketListener;
// Check connection with server
NetResult ifConnected = NetResult::Net_NotYetImplemented;
// Check operands
int operandsActive = 0;
// Check number of messages
int numberOfMessage = 0;

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


int ps2i(String^& dataPstr)
{
	std::wstring dataPstrWstr(dataPstr->Data());
	auto dataInt = std::stoi(dataPstrWstr);
	return dataInt;
}


double ps2d(String^& dataPstr)
{
	std::wstring dataPstrWstr(dataPstr->Data());
	auto dataDouble = std::stod(dataPstrWstr);
	return dataDouble;
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


void PowerSplitClient::MainPage::PageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Clearing server output area
	textBlockInfoOutput->Text = "";
	// Clearing computing output area
	textBlockComputingOutput->Text = "";
}

void PowerSplitClient::MainPage::ConnectButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	if (Network::Initialize()) {
		std::string dataStr = "WinSock API successfully initialized\r\n";
		textBlockInfoOutput->Text += s2ps(dataStr);

		if (socketListener.Create() == NetResult::Net_Success)
		{
			std::string dataStr = "Socket successfully created\r\n";
			textBlockInfoOutput->Text += s2ps(dataStr);

			String^ ipPstr = hostTextBox->Text;
			std::string ipStr = ps2s(ipPstr);
			const char* ipChar = ipStr.c_str();

			String^ portPstr = portTextBox->Text;
			int portInt = ps2i(portPstr);

			if (socketListener.Connect(IPEndpoint(ipChar, portInt)) == NetResult::Net_Success)
			{
				ifConnected = NetResult::Net_Success;

				std::string dataStr = "Succesfully connected to server!\r\n";
				textBlockInfoOutput->Text += s2ps(dataStr);

				uint32_t messageNumber, messageSize;
				std::string messageData;
				messageData = "Hello, Server!";
				messageNumber = ++numberOfMessage;
				messageSize = messageData.size();
				NetPacket packetTest;
				packetTest << messageNumber << messageSize << messageData;

				NetResult result = NetResult::Net_Success;
				while (result == NetResult::Net_Success)
				{
					std::string dataStr = "Attempting to send set of data...\r\n";
					textBlockInfoOutput->Text += s2ps(dataStr);

					result = socketListener.Send(packetTest);
					if (result != NetResult::Net_Success)
						break;

					dataStr = "Data has been sent to server\r\n";
					textBlockInfoOutput->Text += s2ps(dataStr);
					break;
				}
			}
			else
			{
				std::string dataStr = "Failed to connect to server!\r\n";
				textBlockInfoOutput->Text += s2ps(dataStr);

				socketListener.Close();

				ifConnected = NetResult::Net_NotYetImplemented;

				dataStr = "Socket successfully closed\r\n";
				textBlockInfoOutput->Text += s2ps(dataStr);
			}
		}
		else
		{
			std::string dataStr = "Socket failed to create\r\n";
			textBlockInfoOutput->Text += s2ps(dataStr);

			socketListener.Close();

			ifConnected = NetResult::Net_NotYetImplemented;

			dataStr = "Socket successfully closed\r\n";
			textBlockInfoOutput->Text += s2ps(dataStr);
		}
	}
	else {
		std::string dataStr = "WSAStartup failed with error\r\n";
		textBlockInfoOutput->Text += s2ps(dataStr);

		Network::Shutdown();

		dataStr = "WinSock API successfully closed\r\n";
		textBlockInfoOutput->Text += s2ps(dataStr);
	}
}


void PowerSplitClient::MainPage::DisconenctButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	socketListener.Close();

	ifConnected = NetResult::Net_NotYetImplemented;

	std::string dataStr = "Socket successfully closed\r\n";
	textBlockInfoOutput->Text += s2ps(dataStr);

	Network::Shutdown();

	dataStr = "WinSock API successfully closed\r\n";
	textBlockInfoOutput->Text += s2ps(dataStr);
}


void PowerSplitClient::MainPage::CheckBoxClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// clearing Vector checkBoxesActive before using it to avoid copying 
	checkBoxesActive.clear();

	// Array with CheckBox^ checkBoxes
	CheckBox^ checkBoxes[] = { checkBox1, checkBox2, checkBox3, checkBox4, checkBox5, checkBox6, checkBox7, checkBox8 };

	// Checking active check boxes and writing it to vector
	for each (CheckBox ^ checkBox in checkBoxes)
	{
		if (checkBox->IsChecked->Value == true) {
			// change checkBox->Content dataType from String^ to int
			String^ checkBoxContentPStr = checkBox->Content->ToString();
			std::string checkBoxContentStr = ps2s(checkBoxContentPStr);

			// inserting method into vector with active methods
			checkBoxesActive.emplace_back(checkBoxContentStr);
		}
	}
}


void PowerSplitClient::MainPage::SubmitButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Check active connection with server
	if (ifConnected == NetResult::Net_NotYetImplemented)
	{
		std::string dataStr = "Please first connect to server\r\n";
		textBlockInfoOutput->Text += s2ps(dataStr);
	}
	else
	{
		if (checkBoxesActive.size() == 0)
		{
			std::string dataStr = "Please choose any method from checkBoxes\r\n";
			textBlockInfoOutput->Text += s2ps(dataStr);
		}
		else
		{
			// Prepare varibles to sending
			String^ operand1Pstr = operand1TextBox->Text;
			std::string operand1Str = ps2s(operand1Pstr);
			if (operand1Str == "") {
				operand1Str = "0";
			}
			int operand1Int = ps2i(operand1Pstr);
			double operand1Double = ps2d(operand1Pstr);

			String^ operand2Pstr = operand2TextBox->Text;
			std::string operand2Str = ps2s(operand2Pstr);
			if (operand2Str == "") {
				operand2Str = "0";
			}
			int operand2Int = ps2i(operand2Pstr);
			double operand2Double = ps2d(operand2Pstr);

			std::string methodsStr = "";
			for each (std::string checkBoxActive in checkBoxesActive)
			{
				if (methodsStr == "") {
					methodsStr += checkBoxActive;
				}
				else
				{
					methodsStr += " " + checkBoxActive;
				}
			}

			// Prepare sending data
			uint32_t messageNumber, messageSize;
			std::string messageData;
			messageNumber = ++numberOfMessage;
			messageSize = (int)sizeof(messageData);
			messageData = methodsStr;

			//// for int dataType
			//uint32_t operand1, operand2;
			//operand1 = operand1Int; operand2 = operand2Int;

			// for double dataType
			std::string operand1, operand2;
			operand1 = operand1Str; operand2 = operand2Str;

			NetPacket packetToSend;
			packetToSend << messageNumber << messageSize << operand1 << operand2 << messageData;

			// Send the prepared packet with data
			NetResult result = NetResult::Net_Success;
			while (result == NetResult::Net_Success)
			{
				std::string dataStr = "Attempting to send set of data...\r\n";
				textBlockInfoOutput->Text += s2ps(dataStr);

				result = socketListener.Send(packetToSend);
				if (result != NetResult::Net_Success)
					break;

				dataStr = "Data has been sent to server\r\n";
				textBlockInfoOutput->Text += s2ps(dataStr);
				break;
			}

			std::string dataStr = "Data has just been sent to the server\r\n";
			textBlockInfoOutput->Text += s2ps(dataStr);

			// Wait for response data from server
			std::string resultStr;
			NetPacket packetToReceive;
			result = NetResult::Net_Success;
			int numberOfMessages = 0;
			while (result == NetResult::Net_Success)
			{
				result = socketListener.Receive(packetToReceive);
				if (result != NetResult::Net_Success)
					break;

				std::string dataStr = "Data received:\r\n";
				textBlockComputingOutput->Text += s2ps(dataStr);
				try
				{
					packetToReceive >> resultStr;
				}
				catch (NetPacketException& exception)
				{
					std::string dataStr = exception.ToString() + "\r\n";
					textBlockComputingOutput->Text += s2ps(dataStr);
				}
				dataStr = resultStr;
				textBlockComputingOutput->Text += s2ps(dataStr);
				break;
			}

			dataStr = "Data has just been received from the server\r\n";
			textBlockInfoOutput->Text += s2ps(dataStr);
		}
	}
}
