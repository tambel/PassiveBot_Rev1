#include "stdafx.h"
#include "Memory.h"

using namespace std;

namespace Tools
{

#ifdef _WIN32
	HANDLE  Process::process=NULL;
	DWORD Process::thread_id=NULL;
	HWND Process::window=NULL;
	unsigned Process::base_address=0;
	unsigned Process::mouse_x=0;
	unsigned Process::mouse_y=0;
	bool Process::Init()
	{
		cout<<"Process initialization"<<endl;
		CloseHandle(process);
		DWORD id;
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32;
		window=FindWindow("GXWindowClass","World of Warcraft");
		if (window==NULL)
		{
			cout<<"   Window not found"<<endl;
			return false;
		}

		GetWindowThreadProcessId(window,&id);

		process=OpenProcess(PROCESS_ALL_ACCESS ,NULL,id);
		if (process==NULL)
		{
			cout<<"   OpenProcess failed"<<endl;
			return false;
		}

		hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, id);
		if( hModuleSnap == INVALID_HANDLE_VALUE )
		{
			return false;
		}
		me32.dwSize = sizeof( MODULEENTRY32 );
		if( !Module32First( hModuleSnap, &me32 ) )
		{
			return false;
		}
		base_address=(unsigned)me32.modBaseAddr;
		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hModuleSnap != INVALID_HANDLE_VALUE) {
			THREADENTRY32 te;
			FILETIME time={0};
			bool first=true;
			te.dwSize = sizeof(te);
			if (!Thread32First(hModuleSnap, &te))
			{
				return 0;
			} 

			do
			{
				if (te.th32OwnerProcessID==id)
				{
					HANDLE thread=OpenThread(  THREAD_ALL_ACCESS ,FALSE,te.th32ThreadID);
					int error=GetLastError();
					FILETIME ct={0};
					FILETIME et={0};
					FILETIME kt={0};
					FILETIME ut={0};
					GetThreadTimes(thread,&ct,&et,&kt,&ut);
					if (first)
					{
						time=ct;
						thread_id=te.th32ThreadID;
						first=false;
						continue;
					}
					if (CompareFileTime(&time,&ct)>0)
					{
						time=ct;
						thread_id=te.th32ThreadID;

					}
				}
			}
			while (Thread32Next(hModuleSnap, &te));
		}
		CloseHandle( hModuleSnap );
		return true;
	}
	bool Process::FindExistingProcess()
	{
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				if (strcmp(entry.szExeFile, "Wow.exe") == 0)
				{  
					CloseHandle(snapshot);
					return true;
				}
			}
		}
		return false;
	}
	bool Process::LaunchProcess()
	{
		PROCESS_INFORMATION pi = {0};
		STARTUPINFO si = {0};
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32={0};
		me32.dwSize = sizeof( MODULEENTRY32 );
		if (!CreateProcess(WOW_EXECUTABLE_PATH,NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
		{
			TerminateProcess(pi.hProcess,0);
			return false;
		}
		return true;
	}
	wstring Process::ReadString_UTF8(unsigned address, unsigned long length)
	{
		unique_ptr<wchar_t> tmpWCResult;
		unique_ptr<wchar_t> tmpResult;
		unique_ptr<wchar_t> result;
		MEMORY_BASIC_INFORMATION bi={0};
		VirtualQueryEx(process,(void*)address,&bi,sizeof(bi));
		DWORD real_length=0;
		DWORD resLength=0;
		if (length==0)
		{
			length=1024;
		}
		if (bi.RegionSize-(address-(unsigned)bi.BaseAddress)>=length*2)
		{

			real_length=length;
		}
		else
		{
			real_length=(bi.RegionSize-((DWORD)address-(DWORD)bi.BaseAddress))/2;
		}
		tmpWCResult = unique_ptr<wchar_t>(new wchar_t[length]);
		tmpResult = unique_ptr<wchar_t>(new wchar_t[length]);
		bool r_res = ReadProcessMemory(process, (void*)address, tmpWCResult.get(), real_length * 2, NULL)!=0;
		MultiByteToWideChar(65001,0,(LPCCH)tmpWCResult.get(),-1,tmpResult.get(),real_length);
		resLength=wcslen(tmpResult.get())+1;
		result = unique_ptr<wchar_t>(new wchar_t[resLength]);
		memcpy(result.get(),tmpResult.get(),resLength*2);
		if (!r_res || !result.get())
		{
			throw MemoryReadException(address);
		}
		return wstring(result.get());

	}	
	bool Process::ReadRaw(unsigned address, void * buffer,unsigned long length)
	{
		DWORD byte_read;
		bool r_res = ReadProcessMemory(process, (void*)address, buffer, length, &byte_read)!=0;
		if (!r_res || byte_read != length)
		{
			throw MemoryReadException(address);
		}
		return true;
	}
	bool Process::ReadRelRaw(unsigned offest,void * buffer, unsigned long length)
	{
		return ReadRaw(Process::GetProcessBase()+offest,buffer,length);
	}
	string Process::ReadASCII(unsigned address, unsigned long length)
	{
		unsigned buffer_size = 10;
		SIZE_T byte_read;
		MEMORY_BASIC_INFORMATION bi = { 0 };
		VirtualQueryEx(process, (void*)address, &bi, sizeof(bi));
		char * buffer;
		string result = "";
		
		if (length)
		{
			buffer = new char[length];
			BOOL read_result = ReadProcessMemory(process, (void*)address, buffer, length, &byte_read);
			result = string(buffer, byte_read);
		}
		else
		{
			unsigned current_address = address;
			unsigned current_length = 0;
			buffer= new char[buffer_size];
			int part_length;
			bool end = false;
			while ((unsigned)bi.BaseAddress + bi.RegionSize > current_address && !end)
			{
				BOOL read_result = ReadProcessMemory(process, (void*)current_address, buffer, buffer_size, &byte_read);
				if (!read_result || !byte_read)
				{
					break;
				}
				for (part_length = 0; part_length < byte_read; part_length++)
				{
					if (buffer[part_length] == 0)
					{
						end = true;
						//part_length++;
						break;
					}
				}
				current_address += byte_read;
				string part = string(buffer, part_length);
				result = result + part;

			}
		}
		delete[] buffer;
		if (result[0] == 'M' && result[1] == 'a' && result[2] == 'g'  && result[3] == 'i')
		{
			buffer = buffer;
		}
		return result;
	}
	string Process::ReadASCII2(unsigned address, unsigned long length)
	{
		unique_ptr<char> tmp_result;
		unique_ptr<char> result;
		MEMORY_BASIC_INFORMATION bi={0};
		VirtualQueryEx(process,(void*)address,&bi,sizeof(bi));
		DWORD real_length=0;
		DWORD res_length=0;
		DWORD byte_read=0;
		if (length==0)
		{
			length=1024;
		}
		if (bi.RegionSize-(address-(unsigned)bi.BaseAddress)>=length)
		{

			real_length=length;
		}
		else
		{
			real_length=(bi.RegionSize-((DWORD)address-(DWORD)bi.BaseAddress));
		}
		tmp_result = unique_ptr<char>(new char[real_length]);
		bool r_res=ReadProcessMemory(process, (void*)address, tmp_result.get(), real_length, &byte_read)!=0;
		res_length=strlen(tmp_result.get());
		if (res_length > real_length)
		{
			res_length = real_length;
		}
		result = unique_ptr<char>(new char[res_length]);
		strcpy_s(result.get(), res_length, tmp_result.get());
		//strcpy(result.get(),tmp_result.get());
		if (!r_res || !result.get())
		{
			throw MemoryReadException(address);
		}
		string res = string(result.get());
		return res;

	}
	void Process::MoveMouse(unsigned x, unsigned y,bool b,unsigned long time)
	{
		INPUT input={0};
		input.mi.dx=x;
		input.mi.dy=y;
		input.mi.mouseData=0;
		if (b)
			
		input.mi.dwFlags= MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_RIGHTDOWN;
		else
			input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		input.type=INPUT_MOUSE;
		Sleep(time);
		SendInput(1,&input,sizeof(input));
		mouse_x=x;
		mouse_y=y;
	}
	void Process::MouseUp(MouseButton button)
	{
		INPUT input={0};
		input.mi.dx=mouse_x;
		input.mi.dy=mouse_y;
		input.mi.mouseData=0;
		switch (button)
		{
		case MouseButton::LEFT:
			input.mi.dwFlags= MOUSEEVENTF_LEFTUP;
			break;
		case MouseButton::RIGHT:
			input.mi.dwFlags= MOUSEEVENTF_RIGHTUP;
			break;
		case MouseButton::WHEEL:
			break;
		default:
			break;
		}
		input.type=INPUT_MOUSE;
		SendInput(1,&input,sizeof(input));
	}
	void Process::MouseDown(MouseButton button)
	{
		INPUT input={0};
		input.mi.dx=mouse_x;
		input.mi.dy=mouse_y;
		input.mi.mouseData=0;
		switch (button)
		{
		case MouseButton::LEFT:
			input.mi.dwFlags= MOUSEEVENTF_LEFTDOWN;
			break;
		case MouseButton::RIGHT:
			input.mi.dwFlags= MOUSEEVENTF_RIGHTDOWN;
			break;
		case MouseButton::WHEEL:
			break;
		default:
			break;
		}
		input.type=INPUT_MOUSE;
		SendInput(1,&input,sizeof(input));
	}
	void Process::MouseClick( MouseButton button, unsigned long delay)
	{
		MouseDown(button);
		Sleep(delay);
		MouseUp(button);
	}
	void Process::DoubleClick(MouseButton button, unsigned long interval)
	{
		MouseClick(button);
		Sleep(interval);
		MouseClick(button);
	}
	unsigned short Process::ReinterpretKeybardKey(unsigned short button)
	{
		WORD key;
		if (button>=0x41 && button<=0x5A)
		{
			key=button;
		}
		else if (button>=0x30 && button<=0x39)
		{
			key=button;
		}
		switch (button)
		{
		case KeyboardButton::ARROW_RIGHT:
			key=VK_RIGHT;
			break;
		case KeyboardButton::ARROW_LEFT:
			key=VK_LEFT;
			break;
		case KeyboardButton::BACKSPACE:
			key=VK_BACK;
			break;
		case KeyboardButton::AT:
			key=VK_ATTN;
			break;
		case KeyboardButton::PERIOD:
			key=VK_OEM_PERIOD;
			break;
		case KeyboardButton::ALT:
			key=VK_MENU;
			break;
		case KeyboardButton::CONTROL:
			key=VK_CONTROL;
			break;
		case KeyboardButton::LSHIFT:
			key=VK_LSHIFT;
			break;
		case KeyboardButton::ENTER:
			key=VK_RETURN;
			break;
		case KeyboardButton::TAB:
			key = VK_TAB;
			break;
		case KeyboardButton::ESC:
			key = VK_ESCAPE;
			break;
		}
		return key;
	}
	void Process::PushKeyboardButton(unsigned short button)
	{
		INPUT input={0};
		input.type=INPUT_KEYBOARD;
		input.ki.wVk=ReinterpretKeybardKey(button);
		SendInput(1,&input,sizeof(input));
	}
	void Process::ReleaseKeyboardButton(unsigned short button)
	{
		INPUT input={0};
		input.type=INPUT_KEYBOARD;
		input.ki.wVk=ReinterpretKeybardKey(button);
		input.ki.dwFlags=KEYEVENTF_KEYUP;
		SendInput(1,&input,sizeof(input));
	}
	void Process::PressKeyboardButton(unsigned short button,unsigned long delay,unsigned short add_button)
	{
		INPUT input={0};
		input.type=INPUT_KEYBOARD;
		input.ki.wVk=ReinterpretKeybardKey(button);
		if (add_button)
		{
			PushKeyboardButton(add_button);
			SendInput(1,&input,sizeof(input));
			input.ki.dwFlags=KEYEVENTF_KEYUP;
			SendInput(1,&input,sizeof(input));
			ReleaseKeyboardButton(add_button);
		}
		else
		{
			SendInput(1,&input,sizeof(input));
			delay ? Sleep(delay) : delay;
			input.ki.dwFlags=KEYEVENTF_KEYUP;
			SendInput(1,&input,sizeof(input));
		}
		
	}
	HWND Process::GetWindow()
	{
		return window;
	}
	Rect Process::GetClientSize()
	{
		RECT rc={0};
		Rect result={0};
		GetClientRect(window,&rc);
		int e =GetLastError();
		result.heigth=rc.bottom-rc.top;
		result.width=rc.right-rc.left;
		return result;
	}
	unsigned Process::GetPID()
	{

		return GetProcessId(process);
	}
	Language Process::GetCurrentLanguage()
	{
		HKL l = GetKeyboardLayout(thread_id);
		Language lang;
		WORD low = (WORD)l >> 0;
		
		char low2 = low >> 0;
		switch (low2 >> 0)
		{
		case LANG_RUSSIAN:
			lang = Language::RUSSIAN;
			break;
		case LANG_ENGLISH:
			lang = Language::ENGLISH;
			break;
		}
		return lang;
	}
	vector<unsigned> Process::FindInMemory(double value)
	{
		int result;
		int error;
		SIZE_T read = 0;
		unsigned start = Process::base_address;
		unsigned end = 0;
		size_t length = 0;
		MEMORY_BASIC_INFORMATION mbi;
		vector<unsigned>found = vector<unsigned>();
		char * buffer;
		do
		{
			memset(&mbi, 0, sizeof(mbi));
			result = VirtualQueryEx(process, (LPCVOID)start, &mbi, sizeof(mbi));
			if (result)
			{
				end = start + mbi.RegionSize;
				if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE)
				{
					buffer = new char[mbi.RegionSize];
					result = ReadProcessMemory(process, (LPCVOID)mbi.BaseAddress, buffer, mbi.RegionSize, &read);
					if (result && read == mbi.RegionSize)
					{
						unsigned addr = reinterpret_cast<unsigned>(mbi.BaseAddress);
						for (char * p = buffer; p < buffer + mbi.RegionSize; p++)
						{
							if (value == *reinterpret_cast<double*>(p))
							{
								found.push_back(addr);
							}
							addr++;
						}
					}
					delete[] buffer;
				}

			}
			start = end + 1;
		} while (result);
		error = GetLastError();
		return found;
	}
	void Process::WriteRaw(char * buffer, unsigned size, unsigned address)
	{
		SIZE_T written = 0;
		WriteProcessMemory(process, (LPVOID)address, buffer, size, &written);
	}
	void Process::InjectDLL(string dll_path)
	{
		LPVOID LoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
		LPVOID Memory = (LPVOID)VirtualAllocEx(process, NULL, dll_path.length() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (!WriteProcessMemory(process, (LPVOID)Memory, dll_path.c_str(), dll_path.length() + 1, NULL))
		{
			//TODO Handle this
		}
		CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, (LPVOID)Memory, NULL, NULL);

	}
#endif

}