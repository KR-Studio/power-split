//
// MainPage.xaml.h
// Объявление класса MainPage.
//

#pragma once

#include "MainPage.g.h"

namespace PowerSplit
{
	/// <summary>
	/// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void RectangleTextLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void thread_rect_square(Platform::String^ name, double width, double height);
	};
}
