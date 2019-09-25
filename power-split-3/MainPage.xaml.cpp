//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <regex>
#include <future>

using namespace PowerSplit;

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

using namespace Windows::UI::Xaml::Shapes;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

//std::vector<std::thread> threads;
std::vector<std::thread> threads2;
std::mutex mtx;
std::map<String^, String^> squares_map;
TextBlock^ rectangleTextBlocks[5];
int generalSquare = 0;

MainPage::MainPage()
{
	InitializeComponent();
}

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

void threadFunction(std::future<void> futureObj)
{
	std::string dbgString1 = "Thread Start \r\n";
	std::wstring dbgWString1 = s2ws(dbgString1);
	LPCWSTR finalDbgStr1 = dbgWString1.c_str();
	OutputDebugString(finalDbgStr1);
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		std::string dbgString2 = "Doing Some Work \r\n";
		std::wstring dbgWString2 = s2ws(dbgString2);
		LPCWSTR finalDbgStr2 = dbgWString2.c_str();
		OutputDebugString(finalDbgStr2);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	std::string dbgString3 = "Thread End \r\n";
	std::wstring dbgWString3 = s2ws(dbgString3);
	LPCWSTR finalDbgStr3 = dbgWString3.c_str();
	OutputDebugString(finalDbgStr3);
}

void thread_rect_square(Platform::String^ name, double width, double height) {
	std::lock_guard<std::mutex> guard(mtx);
	int squareNum = width * height;
	std::wstring squareNumWstr = std::to_wstring(squareNum);
	String^ squareNumPStr = ref new String(squareNumWstr.c_str());
	std::this_thread::sleep_for(std::chrono::seconds(2));
	squares_map[name] = squareNumPStr;
}

void thread_all_rect_square() {
	std::lock_guard<std::mutex> guard(mtx);
	int squareNum = 0;
	for (auto const& [key, val] : squares_map) {
		std::wstring valuePstrWstr(val->Data());
		int value = std::stoi(valuePstrWstr);
		squareNum += value;
	}
	std::wstring squareNumWstr = std::to_wstring(squareNum);
	String^ squareNumPStr = ref new String(squareNumWstr.c_str());
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::string key = "rectangles";
	std::wstring wsKey = s2ws(key);
	String^ pStrKey = ref new String(wsKey.c_str());
	squares_map[pStrKey] = squareNumPStr;
}

void PowerSplit::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Rectangle^ rectangles[] = { rectangle1, rectangle2, rectangle3, rectangle4 };

	rectangleTextBlocks[0] = rectangle1Text;
	rectangleTextBlocks[1] = rectangle2Text;
	rectangleTextBlocks[2] = rectangle3Text;
	rectangleTextBlocks[3] = rectangle4Text;
	rectangleTextBlocks[4] = rectanglesText;

	std::vector<std::thread> threads;

	for each (Rectangle ^ rectangle in rectangles) {
		std::thread thr(thread_rect_square, rectangle->Name, rectangle->Width, rectangle->Height);
		threads.emplace_back(std::move(thr));
	}

	std::thread thr(thread_all_rect_square);
	threads.emplace_back(std::move(thr));

	for (auto& thread : threads) {
		thread.join();
	}
	
	for each (Rectangle ^ rectangle in rectangles) {
		rectangle->Height = 10;
	}
	

	/*for (auto& [name, square] : squares_map) {
		for (auto& textblock : rectangleTextBlocks) {
			if (name == textblock->Text) {
				textblock->Text = square;
			}
		}
	}*/
}

void PowerSplit::MainPage::RectangleTextLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
}


void PowerSplit::MainPage::ThreadControlsLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
}


void PowerSplit::MainPage::StartTask1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	rectangle1->Height = 150;
	std::thread thr(thread_rect_square, rectangle1->Name, rectangle1->Width, rectangle1->Height);
	threads2.emplace_back(std::move(thr));
	/*for (auto& thread : threads2) {
		thread.join();
	}*/
	//rectangleTextBlocks[0]->Text = squares_map.find(rectangleTextBlocks[0]->Text)->second;
	for (auto& [name, square] : squares_map) {
			if (name == rectangle1Text->Text) {
				rectangle1Text->Text = square;
				std::wstring wsstr(square->Data());//Convert Platform::String to wstring
				int n = std::stoi(wsstr);//Convert String to Int
				generalSquare += n;
			}
	}
}


void PowerSplit::MainPage::StopTask1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Create a std::promise object
	std::promise<void> exitSignal;
	// Fetch std::future object associated with promise
	std::future<void> futureObj = exitSignal.get_future();
	// Starting Thread & move the future object in lambda function by reference
	std::thread th(&threadFunction, std::move(futureObj));
	// Wait for 10 sec
	std::this_thread::sleep_for(std::chrono::seconds(1));
	OutputDebugString((LPCWSTR)"Asking Thread to Stop \r\n");
	// Set the value in promise
	exitSignal.set_value();
	// Wait for thread to join
	th.join();
}


void PowerSplit::MainPage::StartTask2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	rectangle2->Height = 160;
	std::thread thr(thread_rect_square, rectangle2->Name, rectangle2->Width, rectangle2->Height);
	threads2.emplace_back(std::move(thr));
	/*for (auto& thread : threads2) {
		thread.join();
	}*/
	//rectangleTextBlocks[1]->Text = squares_map.find(rectangleTextBlocks[1]->Text)->second;
	for (auto& [name, square] : squares_map) {
		if (name == rectangle2Text->Text) {
			rectangle2Text->Text = square;
			std::wstring wsstr(square->Data());//Convert Platform::String to wstring
			int n = std::stoi(wsstr);//Convert String to Int
			generalSquare += n;
		}
	}
}


void PowerSplit::MainPage::StopTask2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Create a std::promise object
	std::promise<void> exitSignal;
	// Fetch std::future object associated with promise
	std::future<void> futureObj = exitSignal.get_future();
	// Starting Thread & move the future object in lambda function by reference
	std::thread th(&threadFunction, std::move(futureObj));
	// Wait for 10 sec
	std::this_thread::sleep_for(std::chrono::seconds(10));
	OutputDebugString((LPCWSTR)"Asking Thread to Stop \r\n");
	// Set the value in promise
	exitSignal.set_value();
	// Wait for thread to join
	th.join();
}


void PowerSplit::MainPage::StartTask3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	rectangle3->Height = 130;
	std::thread thr(thread_rect_square, rectangle3->Name, rectangle3->Width, rectangle3->Height);
	threads2.emplace_back(std::move(thr));
	/*for (auto& thread : threads2) {
		thread.join();
	}*/
	//rectangleTextBlocks[2]->Text = squares_map.find(rectangleTextBlocks[2]->Text)->second;
	for (auto& [name, square] : squares_map) {
		if (name == rectangle3Text->Text) {
			rectangle3Text->Text = square;
			std::wstring wsstr(square->Data());//Convert Platform::String to wstring
			int n = std::stoi(wsstr);//Convert String to Int
			generalSquare += n;
		}
	}
}


void PowerSplit::MainPage::StopTask3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Create a std::promise object
	std::promise<void> exitSignal;
	// Fetch std::future object associated with promise
	std::future<void> futureObj = exitSignal.get_future();
	// Starting Thread & move the future object in lambda function by reference
	std::thread th(&threadFunction, std::move(futureObj));
	// Wait for 10 sec
	std::this_thread::sleep_for(std::chrono::seconds(10));
	OutputDebugString((LPCWSTR)"Asking Thread to Stop \r\n");
	// Set the value in promise
	exitSignal.set_value();
	// Wait for thread to join
	th.join();
}


void PowerSplit::MainPage::StartTask4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	rectangle4->Height = 140;
	std::thread thr(thread_rect_square, rectangle4->Name, rectangle4->Width, rectangle4->Height);
	threads2.emplace_back(std::move(thr));
	/*for (auto& thread : threads2) {
		thread.join();
	}*/
	//rectangleTextBlocks[3]->Text = squares_map.find(rectangleTextBlocks[3]->Text)->second;
	for (auto& [name, square] : squares_map) {
		if (name == rectangle4Text->Text) {
			rectangle4Text->Text = square;
			std::wstring wsstr(square->Data());//Convert Platform::String to wstring
			int n = std::stoi(wsstr);//Convert String to Int
			generalSquare += n;
		}
	}
}


void PowerSplit::MainPage::StopTask4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Create a std::promise object
	std::promise<void> exitSignal;
	// Fetch std::future object associated with promise
	std::future<void> futureObj = exitSignal.get_future();
	// Starting Thread & move the future object in lambda function by reference
	std::thread th(&threadFunction, std::move(futureObj));
	// Wait for 10 sec
	std::this_thread::sleep_for(std::chrono::seconds(10));
	OutputDebugString((LPCWSTR)"Asking Thread to Stop \r\n");
	// Set the value in promise
	exitSignal.set_value();
	// Wait for thread to join
	th.join();
}


void PowerSplit::MainPage::StartTask5_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::thread thr(thread_all_rect_square);
	threads2.emplace_back(std::move(thr));
	/*for (auto& thread : threads2) {
		thread.join();
	}*/
	//rectangleTextBlocks[4]->Text = squares_map.find(rectangleTextBlocks[4]->Text)->second;
	/*for (auto& [name, square] : squares_map) {
		if (name == rectanglesText->Text) {
			rectanglesText->Text = square;
		}
	}*/
	std::wstring totalSquare = std::to_wstring(generalSquare);
	String^ totalSquarePStr = ref new String(totalSquare.c_str());
	rectanglesText->Text = totalSquarePStr;
}


void PowerSplit::MainPage::StopTask5_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Create a std::promise object
	std::promise<void> exitSignal;
	// Fetch std::future object associated with promise
	std::future<void> futureObj = exitSignal.get_future();
	// Starting Thread & move the future object in lambda function by reference
	std::thread th(&threadFunction, std::move(futureObj));
	// Wait for 10 sec
	std::this_thread::sleep_for(std::chrono::seconds(10));
	OutputDebugString((LPCWSTR)"Asking Thread to Stop \r\n");
	// Set the value in promise
	exitSignal.set_value();
	// Wait for thread to join
	th.join();
}
