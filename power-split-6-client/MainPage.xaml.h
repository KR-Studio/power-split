//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace PowerSplitClient
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void ConnectBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void DisconenctBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void StartTask1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void StartTask2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void StartTask3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void StartTask4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
