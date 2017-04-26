#pragma once
#include "Exceptions.h"
using namespace std;
#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
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
		ENGLISH = 0,
		RUSSIAN = 1
	};
	enum KeyboardButton
	{
		ARROW_RIGHT = 150,
		ARROW_LEFT = 151,
		AT = 110,
		PERIOD = 120,
		BACKSPACE = 130,
		LSHIFT = 140,
		ALT = 141,
		CONTROL = 142,
		ENTER = 160,
		TAB=143,
		ESC=170

	};
	enum MouseButton
	{
		LEFT = 1,
		RIGHT = 2,
		WHEEL = 3
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
		
	public:
#ifdef _WIN32
		static HWND GetWindow();
#endif
		static bool Init();
		static unsigned GetProcessBase();
		static wstring ReadString_UTF8(unsigned address, unsigned long  length);
		template <typename T>
		static T Read(unsigned address)
		{

			T result;

#ifdef _WIN32
			unsigned long byte_read;
			if (ReadProcessMemory(process, (void*)address, &result, sizeof(T), &byte_read))
			{
				if (byte_read == sizeof(T))
				{
					return result;
				}
			}
			throw MemoryReadException(address);
#endif
		}
		template <typename T>
		static T ReadRel(unsigned offset)
		{
			T result;
			try
			{
				result = Read<T>(base_address + offset);
			}
			catch (MemoryReadException e)
			{
				throw;
			}
			return result;
		}
		static void MouseUp(MouseButton button);
		static void MouseDown(MouseButton button);
		static bool ReadRaw(unsigned address, void * buffer, unsigned long length);
		static bool ReadRelRaw(unsigned offest, void * buffer, unsigned long length);
		static string ReadASCII2(unsigned address, unsigned long length);
		static string ReadASCII(unsigned address, unsigned long length);

		static void MoveMouse(unsigned x, unsigned y,bool b=false, unsigned long time = 50);
		static void MouseClick(MouseButton button, unsigned long delay = 50);

		static void DoubleClick(MouseButton button, unsigned long interval = 20);
		static void PushKeyboardButton(unsigned short button);
		static void ReleaseKeyboardButton(unsigned short button);
		static void PressKeyboardButton(unsigned short button, unsigned long delay = 50, unsigned short add_button = 0);
		static void MultipleKeyboardButtonPush(unsigned short button, unsigned quantity, unsigned long interval);
		static void TypeByKeyboard(string & string);
		static Language GetCurrentLanguage();
		static void SetLanguage(Language lang);
		static bool FindExistingProcess();
		static bool LaunchProcess();
		static Rect GetClientSize();
		static unsigned GetPID();
		static vector<unsigned> FindInMemory(double value);
		template <typename T>
		static void Write(T value, unsigned address)
		{
			SIZE_T wrote=0;
			bool result = WriteProcessMemory(process, (LPVOID)address,&value, sizeof(T), &wrote);
			if (!result || wrote != sizeof(T))
				throw MemoryWriteException(address);
		}
		static void WriteRaw(char * buffer, unsigned size, unsigned address);
		static void InjectDLL(string dll_path);



	};
}

