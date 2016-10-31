#pragma once
//#include "FrameManager.h"
#include "AutoItX3_DLL.h"
#include "AddonInteractor.h"



enum ExitCode
{
	LoggedOut=1
};

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
	static bool Logout() { return AddonInteractor::Logout(); }
	static bool ManualLogout();
};


