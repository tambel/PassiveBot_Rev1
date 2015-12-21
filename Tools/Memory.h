#pragma once
#include <string>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace Tools
{
	struct Rect
	{
		unsigned width;
		unsigned heigth;
	};
	enum Language
	{
		ENGLISH=0,
		RUSSIAN=1
	};
	enum KeyboardButton
	{
		ARROW_RIGHT=150,
		ARROW_LEFT=151,
		AT=110,
		PERIOD=120,
		BACKSPACE=130,
		LSHIFT=140,
		ALT=141,
		CONTROL=142,
		ENTER=160
		
	};
	enum MouseButton
	{
		LEFT=1,
		RIGHT=2,
		WHEEL=3
	};
	using namespace std;
	class  Process
	{
	protected:
#ifdef _WIN32
		#define WOW_EXECUTABLE_PATH "E:\\World of Warcraft\\Wow.exe"
		static HANDLE process;
		static HWND window;
		static unsigned mouse_x;
		static unsigned mouse_y;
		static DWORD thread_id;
#endif

		static unsigned base_address;
		static vector<unsigned short>pressed_buttons;
		static unsigned short ReinterpretKeybardKey(unsigned short button);
		static bool IsButtonPressed(unsigned short button);
		static void MouseUp(MouseButton button);
		static void MouseDown(MouseButton button);
	public:
#ifdef _WIN32
		static HWND GetWindow();
#endif
		static bool Init();
		static unsigned GetProcessBase();
		static wchar_t * ReadString_UTF8(unsigned address, unsigned long  length);
		static unsigned ReadUInt(unsigned address);
		static char ReadByte(unsigned address);
		static bool ReadRaw(unsigned address, void * buffer, unsigned long length);
		static bool ReadRelRaw(unsigned offest,void * buffer, unsigned long length);
		static unsigned ReadRelUInt(unsigned offset);
		static char * ReadASCII(unsigned address,unsigned long length);
		static float ReadFloat(unsigned address);
		static float ReadRelFloat(unsigned offset);
		static void MoveMouse(unsigned x,unsigned y,unsigned long time=50);
		static void MouseClick(MouseButton button,unsigned long delay=50);
		
		static void DoubleClick(MouseButton button,unsigned long interval=20);
		static void PushKeyboardButton(unsigned short button);
		static void ReleaseKeyboardButton(unsigned short button);
		static void PressKeyboardButton(unsigned short button,unsigned long delay=50,unsigned short add_button=0);
		static void MultipleKeyboardButtonPush(unsigned short button,unsigned quantity,unsigned long interval);
		static void TypeByKeyboard(char * string);
		static Language GetCurrentLanguage();
		static void SetLanguage(Language lang);
		static bool FindExistingProcess();
		static bool LaunchProcess();
		static char ReadRelByte(unsigned offset);
		static bool ReadRelBool(unsigned offset);
		static bool ReadBool(unsigned address);
		static Rect GetClientSize();
		

		
	};
}
