//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <regex>
#include <string>

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

std::vector<std::thread> threads;
std::mutex mtx;
std::map<String^, String^> squares_map;
TextBlock^ rectangleTextBlocks[5];

MainPage::MainPage()
{
	InitializeComponent();
}

void thread_rect_square(Platform::String^ name, double width, double height) {
	std::lock_guard<std::mutex> guard(mtx);
	int squareNum = width * height;
	std::wstring squareNumWstr = std::to_wstring(squareNum);
	String^ squareNumPStr = ref new String(squareNumWstr.c_str());
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
	squares_map["rectangles"] = squareNumPStr;
}

void finalise() {
	/*for (auto& thread : threads) {
		thread.join();
	}*/

	for (auto& [name, square] : squares_map) {
		for (auto& textblock : rectangleTextBlocks) {
			if (name == textblock->Text) {
				textblock->Text = square;
			}
		}
	}
}


void PowerSplit::MainPage::RectangleTextLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void PowerSplit::MainPage::ThreadControlsLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	rectangleTextBlocks[0] = rectangle1Text;
	rectangleTextBlocks[1] = rectangle2Text;
	rectangleTextBlocks[2] = rectangle3Text;
	rectangleTextBlocks[3] = rectangle4Text;
	rectangleTextBlocks[4] = rectanglesText;

	//std::vector<std::thread> threads;

	/*for each (Rectangle ^ rectangle in rectangles) {
		std::thread thr(thread_rect_square, rectangle->Name, rectangle->Width, rectangle->Height);
		threads.emplace_back(std::move(thr));
	}*/

	
}


void PowerSplit::MainPage::StartTask1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::thread thr(thread_rect_square, rectangle1->Name, rectangle1->Width, rectangle1->Height);
	threads.emplace_back(std::move(thr));
	finalise();
}


void PowerSplit::MainPage::StopTask1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void PowerSplit::MainPage::StartTask2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::thread thr(thread_rect_square, rectangle2->Name, rectangle2->Width, rectangle2->Height);
	threads.emplace_back(std::move(thr));
	finalise();
}


void PowerSplit::MainPage::StopTask2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void PowerSplit::MainPage::StartTask3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::thread thr(thread_rect_square, rectangle3->Name, rectangle3->Width, rectangle3->Height);
	threads.emplace_back(std::move(thr));
	finalise();
}


void PowerSplit::MainPage::StopTask3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void PowerSplit::MainPage::StartTask4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::thread thr(thread_rect_square, rectangle4->Name, rectangle4->Width, rectangle4->Height);
	threads.emplace_back(std::move(thr));
	finalise();
}


void PowerSplit::MainPage::StopTask4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void PowerSplit::MainPage::StartTask5_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::thread thr(thread_all_rect_square);
	threads.emplace_back(std::move(thr));
	finalise();
}


void PowerSplit::MainPage::StopTask5_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
