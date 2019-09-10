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
std::map<Rectangle^, String^> squares_map;

MainPage::MainPage()
{
	InitializeComponent();
}

void thread_proc(Rectangle^ rectangle) {
	std::lock_guard<std::mutex> guard(mtx);
	int squareNum = rectangle->Width * rectangle->Height;
	std::wstring squareNumWstr = std::to_wstring(squareNum);
	String^ squareNumPStr = ref new String(squareNumWstr.c_str());
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	squares_map[rectangle] = squareNumPStr;
}

void PowerSplit::MainPage::RectangleTextLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Rectangle^ rectangles[] = { rectangle1, rectangle2, rectangle3, rectangle4 };
	std::vector<std::thread> threads;

	for each (Rectangle ^ rectangle in rectangles) {
		std::thread thr(thread_proc, rectangle);
		thr.join();
	}

	for (auto& thr : threads) {
	}
}
