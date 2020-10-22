#ifndef WINDOW_H
#define WINDOW_H

class Window
{
	public:
		Window();
		~Window();

		// Public access methods
		static void Format(int color);
		static unsigned Modulo(int a, unsigned b);
		static void ShowCursor(bool visible);
};

#endif