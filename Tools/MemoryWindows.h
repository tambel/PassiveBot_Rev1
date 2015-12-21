#include "MemLib.h"

namespace Tools
{
	 static class WindowsProcess: protected Process
	{

	public:
		 static wstring ReadString_UTF8(void *address, DWORD length);
	};
}