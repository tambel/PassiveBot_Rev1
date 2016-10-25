#pragma once
#include "FrameManager.h"
#include "AutoItX3_DLL.h"
/*struct GameStartParam
{
	bool debug;
	char * login;
	char * password;
	wchar_t * char_name;
};*/
struct GameStartParam
{
	bool debug;
	string  login;
	string  password;
	wstring  char_name;
	int (*working_func)();
};
class GameInteractor
{

public:
	GameInteractor(void);
	~GameInteractor(void);
	static bool Login(string & login, string & password);
	static bool StartClient();
	static bool WaitWhileConnecting();
	static bool IsInWorld();
	static void CheckForPromoFrames();
	static bool SelectCharacterAlternative(wstring & character_name);
	static bool SelectCharacter(wstring & character_name);
	static bool Start(GameStartParam * param);
	static void Close();
};


