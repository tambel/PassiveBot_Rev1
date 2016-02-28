#pragma once

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
};
class GameInteractor
{

public:
	GameInteractor(void);
	~GameInteractor(void);
	static bool isWorldLoading();
	static bool Login(string & login, string & password);
	static bool StartClient();
	static bool IsLoaded();
	static bool IsLoggingIn();
	static bool IsCharacterSelecting();
	static bool IsInWorld();
	static bool WaitUntilClientLoad();
	static bool WaitForAuthentification();
	static void CheckForPromoFrames();
	static void Test();
	static bool SelectCharacter(wstring & name);
	static bool Start(GameStartParam * param);
	static void Close();
};


