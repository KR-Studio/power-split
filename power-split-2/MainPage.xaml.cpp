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
TextBlock^ textblocks[4];
// std::vector<TextBlock^> textblocks;
TextBlock^ textblocks[4];
/*
std::map<String^, String^> textblocks_map;
std::map<TextBlock^, String^> textblocks_map2;
*/

MainPage::MainPage()
{
	InitializeComponent();
}

void PowerSplit::MainPage::thread_rect_square(Platform::String^ name, double width, double height) {
	//std::lock_guard<std::mutex> guard(mtx);
	int squareNum = width * height;
	std::wstring squareNumWstr = std::to_wstring(squareNum);
	String^ squareNumPStr = ref new String(squareNumWstr.c_str());
	/*
	String^ nameTextBlock = name + "Text";
	squares_map[name] = squareNumPStr;*/
	/*
	String^ textBlockText = name;
	squares_map[name] = squareNumPStr;
	textblocks_map.insert({ textBlockText, squareNumPStr });
	*/
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	String^ nameTextBlock = name + "Text";
	//for (auto &textblock : textblocks) {
		//if ((nameTextblock) == textblock->Name) {
			//textblock->Text = "etttsstststt";
		//}
	//}
	// rectangle1Text->Text = "sheeeeesh!";

     textblocks[0]->Text = "sheeesh1";
	//rectanglesText->Text = "sheesh";
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
	
	textblocks[0] = rectangle1Text;
	textblocks[1] = rectangle2Text;
	textblocks[2] = rectangle3Text;
	textblocks[3] = rectangle4Text;

    /*
	textblocks_map2.insert({ rectangle1Text, rectangle1Text->Text });
	textblocks_map2.insert({ rectangle2Text, rectangle2Text->Text });
	textblocks_map2.insert({ rectangle3Text, rectangle3Text->Text });
	textblocks_map2.insert({ rectangle4Text, rectangle4Text->Text });
	/*textblocks[0] = rectangle1text;
	textblocks[1] = rectangle2text;
	textblocks[2] = rectangle3text;
	textblocks[3] = rectangle4text;*/
    */
	std::vector<std::thread> threads;

	for each (Rectangle^ rectangle in rectangles) {
		std::thread thr(&PowerSplit::MainPage::thread_rect_square, rectangle->Name, rectangle->Width, rectangle->Height);
		threads.emplace_back(std::move(thr));
	}

	for (auto& [key, val] : textblocks_map2) {
		//if (textblocks_map.find(val) != textblocks_map.end) {
			key->Text = textblocks_map.find(val)->second;
		//}/
	}

	std::thread thr(thread_all_rect_square);
	threads.emplace_back(std::move(thr));

	for (auto& thread : threads) {
		thread.join();
	}
	
}
