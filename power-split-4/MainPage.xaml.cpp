//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <string>
#include <numeric>
#include <time.h>
#include <bitset>

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
std::vector<int> checkBoxesActive = { 0 };

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


void PowerSplit::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
}

void buttonStateChange(Windows::UI::Xaml::Controls::Button^ btnName, bool state)
{
	btnName->IsEnabled = state;
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
	for each (CheckBox^ checkBox in checkBoxes)
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
		checkBoxesActiveText = "There are no active processor cores here";
		buttonStateChange(calculateBtn, false);
	}
	else {
		buttonStateChange(calculateBtn, true);
		for (auto& checkBoxActive : checkBoxesActive) // access by reference to avoid copying
		{
			// change checkBoxActive dataType from int to String^
			std::wstring checkBoxActiveWstr = std::to_wstring(checkBoxActive);
			String^ checkBoxActivePStr = ref new String(checkBoxActiveWstr.c_str());

			// adding information about currently active processor ids into checkBoxesActiveText
			if (checkBoxesActiveText == "") {
				checkBoxesActiveText += "Active processor cores: ";
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


void PowerSplit::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Start execution time measurement
	clock_t tStart = clock();
	
	
	HANDLE process = GetCurrentProcess();

	std::string bitsetStr = "000000000000";
	for (auto& checkBoxActive : checkBoxesActive) // access by reference to avoid copying
	{
		bitsetStr[checkBoxActive] = '1';
	}
	std::reverse(bitsetStr.begin(), bitsetStr.end());

	//std::string bits = "0b" + bitsetStr;

	std::bitset<16> bits(bitsetStr);
	int bitset = (int)(bits.to_ulong());;
	DWORD_PTR processAffinityMask = bitset;

	BOOL success = SetProcessAffinityMask(process, processAffinityMask);

	// Initialize random numbers vector and populate it (needs to be replaced with function call)
	auto vectorSizePstr = textBoxArray->Text->ToString();
	std::wstring vectorSizePstrWstr(vectorSizePstr->Data());
	int vectorSize = std::stoi(vectorSizePstrWstr);
	std::vector<int> v(vectorSize);
	std::generate(v.begin(), v.end(), std::rand);

	std::size_t const parts_num = v.size() / vectorSize;


	auto partsNumPstr = textBoxThreads->Text->ToString();
	std::wstring partsNumPstrWstr(partsNumPstr->Data());
	int partsNum = std::stoi(partsNumPstrWstr);

	std::vector<std::vector<int>> parts(partsNum);

	/*int partSize = std::round(vectorSize / partsNum);

	int lastPos = 0;

	for (int i = 0; i < vectorSize; i++) {
		if (lastPos <= vectorSize) {
			std::vector<int> split(v[lastPos], v[lastPos + partSize]);
			parts[i] = split;
			lastPos += partSize;
		}
		
	}*/

	//TODO: rewrite for parallel calculations
	double average = std::accumulate(v.begin(), v.end(), 0.0) / v.size();

	// Convert calculated average to Platform::String and output it
	std::wstring averageWstr = std::to_wstring(average);
	String^ averagePstr = ref new String(averageWstr.c_str());
	textBlockOutput->Text = averagePstr;

	/*std::thread th(partial_sum);
	threads.emplace_back(std::move(thr));*/

	/*for (auto& thread : threads) {
		thread.join();
	}*/


	//Output execution time
	double execTime = (double)(clock() - tStart) / CLOCKS_PER_SEC;
	std::string execTimeStr = "/n Execution time: " + std::to_string(execTime) + "ms";
	std::wstring execTimeWstr = s2ws(execTimeStr);
	String^ execTimePstr = ref new String(execTimeWstr.c_str());
	textBlockOutput->Text += execTimePstr;
}
