//
// MainPage.xaml.cpp
// Реализация класса MainPage.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <regex>

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

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x419

std::mutex mtx;
std::map<String^, String^> squares_map;
std::vector<TextBlock^> textblocks;

MainPage::MainPage()
{
	InitializeComponent();
}

void thread_rect_square(String^ name, int width, int height) {
	std::lock_guard<std::mutex> guard(mtx);
	int squareNum = width * height;
	std::wstring squareNumWstr = std::to_wstring(squareNum);
	String^ squareNumPStr = ref new String(squareNumWstr.c_str());
	String^ nameTextBlock = name + "Text";
	for (auto textblock : textblocks) {
		//if ((nameTextblock) == textblock->Name) {
			textblock->Text = nameTextBlock;
		//}
	}
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
}

void PowerSplit::MainPage::RectangleTextLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Rectangle^ rectangles[] = { rectangle1, rectangle2, rectangle3, rectangle4 };
	textblocks.push_back(rectangle1Text);
	textblocks.push_back(rectangle2Text);
	textblocks.push_back(rectangle3Text);
	textblocks.push_back(rectangle4Text);
	std::vector<std::thread> threads;

	for each (Rectangle^ rectangle in rectangles) {
		std::thread thr(thread_rect_square, rectangle->Name ,rectangle->Width, rectangle->Height);
		threads.emplace_back(std::move(thr));
	}
	std::thread thr(thread_all_rect_square);
	threads.emplace_back(std::move(thr));
	for (auto& thread : threads) {
		thread.join();
	}
}
