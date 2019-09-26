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
		void PageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox5_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox6_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox7_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox8_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ConnectBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void DisconenctBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SubmitBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
