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

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x419

MainPage::MainPage()
{
	InitializeComponent();
}


void PowerSplit::MainPage::IncreaseClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	String^ counterPStr = counter->Text;
	std::wstring counterPstrWstr(counterPStr->Data());
	int counterNum = std::stoi(counterPstrWstr);
	if (counterNum < 100) {
		counterNum = counterNum + 1;
		counterSlider->Value = counterSlider->Value + 1;
	}
	else {
	}
	std::wstring counterNumWstr = std::to_wstring(counterNum);
	String^ counterNumPStr = ref new String(counterNumWstr.c_str());
	counter->Text = counterNumPStr;
}


void PowerSplit::MainPage::DecreaseClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	String^ counterPStr = counter->Text;
	std::wstring counterPstrWstr(counterPStr->Data());
	int counterNum = std::stoi(counterPstrWstr);
	if (counterNum > 0) {
		counterNum = counterNum - 1;
		counterSlider->Value = counterSlider->Value - 1;
	}
	else {
	}
	std::wstring counterNumWstr = std::to_wstring(counterNum);
	String^ counterNumPStr = ref new String(counterNumWstr.c_str());
	counter->Text = counterNumPStr;
}


void PowerSplit::MainPage::SliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	String^ counterPStr = counter->Text;
	std::wstring counterPstrWstr(counterPStr->Data());
	int counterNum = std::stoi(counterPstrWstr);
	counterNum = counterSlider->Value;
	std::wstring counterNumWstr = std::to_wstring(counterNum);
	String^ counterNumPStr = ref new String(counterNumWstr.c_str());
	counter->Text = counterNumPStr;
}


void PowerSplit::MainPage::RadioChecked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (radioButton1->IsChecked->Value == true) {
		inputRadioText->Text = radioButton1->Content + " is activated";
	} else if(radioButton2->IsChecked->Value == true) {
		inputRadioText->Text = radioButton2->Content + " is activated";
	} else if (radioButton3->IsChecked->Value == true) {
		inputRadioText->Text = radioButton3->Content + " is activated";
	}
}


void PowerSplit::MainPage::CheckBoxClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	CheckBox^ checkBoxes[] = { checkBoxButton1, checkBoxButton2,
											 checkBoxButton3 };
	String^ activeCheckBox = "";
	for each (CheckBox ^ checkBox in checkBoxes)
	{
		if (checkBox->IsChecked->Value == true) {
			if (activeCheckBox == "") {
				activeCheckBox += checkBox->Content;
			}
			else {
				activeCheckBox += ", " + checkBox->Content;
			}
		}

	}
	if (activeCheckBox == "") {
		inputCheckBoxText->Text = "There is no active CheckBox here";
	}
	else {
		inputCheckBoxText->Text = activeCheckBox + " is activated";
	}
}
