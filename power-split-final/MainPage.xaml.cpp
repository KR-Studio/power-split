//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <string>
#include <cmath>

using namespace PowerSplitFinal;

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

std::wstring s2ws(const std::string& outputDataStr)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &outputDataStr[0], (int)outputDataStr.size(), NULL, 0);
	std::wstring dataWstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &outputDataStr[0], (int)outputDataStr.size(), &dataWstr[0], size_needed);
	return dataWstr;
}


String^ s2ps(const std::string& outputDataStr)
{
	std::wstring dataWstr = s2ws(outputDataStr);
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
	auto outputDataStr = ws2s(dataPstrWstr);
	return outputDataStr;
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


MainPage::MainPage()
{
	InitializeComponent();
}


// Define type for f(x)
typedef double(*pointFunction)(double);

double FunctionV3(double x) {
	return (3 * sqrt(x) * (1 + sqrt(x)));
}

double FunctionV8(double x) {
	return (exp(x) * sqrt(1 - exp(x)));
}


double simpsonIntegral(pointFunction f, double a, double b, int n) {
	const double h = (b - a) / n;
	double x1 = 0, x2 = 0;
	for (int i = 1; i < n; i += 2) {
		x1 += f(a + i * h);
		x2 += f(a + (i + 1) * h);
	}
	return h / 3 * (f(a) + 4 * x1 + 2 * x2);
}


double rectangleIntegral(pointFunction f, double a, double b, int n) {
	double x, h;
	double sum = 0.0;
	double fx;
	h = (b - a) / n;  // step
  
	for (int i = 0; i < n; i++) {
		x = a + i*h;
		fx = f(x);
		sum += fx;
	}
	return (sum * h); // the approximate value of the integral is equal to the sum of the areas of the rectangles
}


void PowerSplitFinal::MainPage::PageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Clearing output area
	textBlockOutput->Text = "";
}


void PowerSplitFinal::MainPage::CalculateButtonClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	double leftLimitDouble = 0, rightLimitDouble = 0, epsDouble = 0;
	double yFinalDouble = 0, yDouble = 0;
	int steps = 1; // initial number of steps

	String^ leftLimitPstr = textBoxLeftLimit->Text;
	leftLimitDouble = ps2d(leftLimitPstr);
	String^ rightLimitPstr = textBoxRightLimit->Text;
	rightLimitDouble = ps2d(rightLimitPstr);
	String^ epsPstr = textBoxAccuracy->Text;
	epsDouble = ps2d(epsPstr);

	if (radioButtonMethod1->IsChecked->Value == true)
	{
		yFinalDouble = simpsonIntegral(FunctionV3, leftLimitDouble, rightLimitDouble, steps); // first approximation for integral
		do {
			yDouble = yFinalDouble;     // next approximation
			steps = 2 * steps;  // double the number of steps
						// i.e halving the step value
			yFinalDouble = simpsonIntegral(FunctionV3, leftLimitDouble, rightLimitDouble, steps);
			std::string outputDataStr = "Intermediate result: " + std::to_string(yFinalDouble) + "\r\n";
			textBlockOutput->Text += s2ps(outputDataStr);
		} while (fabs(yFinalDouble - yDouble) > epsDouble);  // comparison of approximations with a given accuracy
	}
	else
	{
		yFinalDouble = rectangleIntegral(FunctionV8, leftLimitDouble, rightLimitDouble, steps); // first approximation for integral
		do {
			yDouble = yFinalDouble;     // next approximation
			steps = 2 * steps;  // double the number of steps
						// i.e halving the step value
			yFinalDouble = rectangleIntegral(FunctionV8, leftLimitDouble, rightLimitDouble, steps);
			std::string outputDataStr = "Intermediate result: " + std::to_string(yFinalDouble) + "\r\n";
			textBlockOutput->Text += s2ps(outputDataStr);
		} while (fabs(yFinalDouble - yDouble) > epsDouble);  // comparison of approximations with a given accuracy
	}

	std::string outputDataStr = "Result: " + std::to_string(yFinalDouble) + "\r\n" + "\r\n";
	textBlockOutput->Text += s2ps(outputDataStr);
}
