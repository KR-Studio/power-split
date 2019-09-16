//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

// Vector with ids of processors that have activeState in checkBoxes
std::vector<int> checkBoxesActive = { 1 };

MainPage::MainPage()
{
	InitializeComponent();
}


void PowerSplit::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
}


void PowerSplit::MainPage::CheckBoxClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// clearing Vector checkBoxesActive before using it to avoid copying 
	checkBoxesActive.clear();

	// Array with CheckBox^ checkBoxes
	CheckBox^ checkBoxes[] = { checkBox1, checkBox2, checkBox3, checkBox4, checkBox5, checkBox6, 
		checkBox7, checkBox8, checkBox9, checkBox10, checkBox11, checkBox12 };
	// String^ that is displayed in textBlockActiveProcessors
	String^ checkBoxesActiveText = "";

	// checking active processors
	for each (CheckBox ^ checkBox in checkBoxes)
	{
		if (checkBox->IsChecked->Value == true) {
			// change checkBox->Content dataType from String^ to int
			auto checkBoxContentPStr = checkBox->Content->ToString();
			std::wstring checkBoxContentPstrWstr(checkBoxContentPStr->Data());
			int processorId = std::stoi(checkBoxContentPstrWstr);

			// inserting processor id into vector with active processor ids
			checkBoxesActive.emplace_back(processorId);
		}
	}

	// writing text about currently active processor ids
	if (checkBoxesActive.empty()) {
		checkBoxesActiveText = "There is no active Processors here";
	}
	else {
		for (auto& checkBoxActive : checkBoxesActive) // access by reference to avoid copying
		{
			// change checkBoxActive dataType from int to String^
			std::wstring checkBoxActiveWstr = std::to_wstring(checkBoxActive);
			String^ checkBoxActivePStr = ref new String(checkBoxActiveWstr.c_str());

			// adding information about currently active processor ids into checkBoxesActiveText
			if (checkBoxesActiveText == "") {
				checkBoxesActiveText += "Active processors: ";
				checkBoxesActiveText += checkBoxActivePStr;
			}
			else {
				checkBoxesActiveText += ", " + checkBoxActivePStr;
			}
		}
	}

	// changing text in textBlockActiveProcessors with currently active processor ids
	textBlockActiveProcessors->Text = checkBoxesActiveText;
}
