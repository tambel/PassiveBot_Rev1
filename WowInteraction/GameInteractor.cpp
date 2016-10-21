#include "stdafx.h"
using namespace Tools;
using namespace WowOffsets2;

GameInteractor::GameInteractor(void)
{
}
GameInteractor::~GameInteractor(void)
{
}
bool GameInteractor::Login(string & login, string  & password)
{
	cout << "Logging in" << endl;
	FrameManager::EnumAllFrames();

	Frame * email_frame = FrameManager::FindFrameByBorders(0.220152885f, 0.243763477f, 0.372026503f, 0.499651313f);

	if (!email_frame)
	{
		cout << "Email frame not found";
		return false;
	}
	email_frame->MoveMouseToFrameAndClick(1000);
	Process::MultipleKeyboardButtonPush(KeyboardButton::ARROW_RIGHT, 20, 50);
	Process::MultipleKeyboardButtonPush(KeyboardButton::BACKSPACE, 40, 40);
	Sleep(1000);
	Process::SetLanguage(Language::ENGLISH);
	Sleep(1000);
	Process::TypeByKeyboard(login);
	Process::PushKeyboardButton(KeyboardButton::TAB);
	Sleep(1000);
	Process::TypeByKeyboard(password);
	Sleep(1000);
	Process::PressKeyboardButton(KeyboardButton::ENTER);
	Sleep(1000);
	return true;
}
bool GameInteractor::StartClient()
{
	unsigned attempts_count = 10;
	if (!Process::FindExistingProcess())
	{
		cout << "Launching new game process" << endl;
		Process::LaunchProcess();
		Sleep(20000);
	}
	cout << "Game process found" << endl;
	unsigned attempts = 0;
	while (!Process::Init() && attempts < attempts_count)
	{
		cout << "Retry" << endl;
		Sleep(5000);
		attempts++;
	}
	if (attempts == attempts_count)
	{
		cout << "Attempts limit is reached" << endl;
		return false;
	}
	if (!WaitUntilClientLoad())
	{
		return false;
	}
	return true;

}
bool GameInteractor::IsLoaded()
{
	return Process::ReadRel<char>(WowOffsets::Client::Loaded) != 0;
}
bool GameInteractor::IsCharacterSelecting()
{
	FrameManager::EnumAllFrames();
	Frame * char_select = FrameManager::FindFrameByName("CharSelectCharacterButton1");

	if (!char_select)
	{
		return false;
	}
	if (!char_select->IsVisible())
	{
		return false;
	}
	return true;

}
bool GameInteractor::WaitWhileConnecting()
{
	while (Process::ReadRel<bool>(WowOffsets2::Client::Connecting) && !Process::ReadRel<bool>(WowOffsets2::Client::CharSelecting))
	{
		

			
		cout << "CONNECTING..."<<'\r' << flush;
		Sleep(100);
	}
	if (Process::ReadRel<bool>(WowOffsets2::Client::Connecting) && Process::ReadRel<bool>(WowOffsets2::Client::CharSelecting))
	{
		cout << "Connection succesfull!" << endl;
		return true;
	}
	else
	{
		cout << "Connection failed!" << endl;
		return false;

	}
}
bool GameInteractor::WaitUntilClientLoad()
{
	cout << "Waiting for client load..." << endl;
	unsigned attempts = 0;
	while (!IsLoaded() && attempts < 6000)
	{
		Sleep(10);
		attempts++;
	}
	if (IsLoaded())
	{
		cout << "Done" << endl;
		return true;
	}
	else
	{
		cout << "Client not loaded" << endl;
		return false;
	}

}
bool GameInteractor::WaitForAuthentification()
{
	cout << "Waiting for client authentification..." << endl;
	unsigned attempts = 0;
	while (Process::ReadRel<char>(WowOffsets::Client::Unavalible) && attempts < 6000)
	{
		Sleep(10);
		attempts++;
	}
	Sleep(10000);
	if (!Process::ReadRel<char>(WowOffsets::Client::Unavalible))
	{
		cout << "Done" << endl;
		return true;
	}
	else
	{
		cout << "Authentification failed" << endl;
		return false;
	}
}
void GameInteractor::CheckForPromoFrames()
{
	cout << "Checking for promo frames" << endl;
	FrameManager::EnumAllFrames();
	
	Frame *starter = FrameManager::FindFrameByName("StarterEditionPopUpExitButton");
	unsigned attempts = 0;
	if (!starter)
	{
		return;
	}
	while (!starter->IsVisible() && attempts < 1000)
	{
		Sleep(10);
		attempts++;
	}
	if (starter->IsVisible())
	{

		starter->MoveMouseToFrame();
		Sleep(5000);
		Process::MouseClick(MouseButton::LEFT, 100);
		cout << "\"StarterEditionPopUpExitButton\" frame found and closed" << endl;
	}
	if (starter->IsVisible())
	{
		return;
	}
}
void GameInteractor::Test()
{
}
bool GameInteractor::SelectCharacterAlternative(wstring & name)
{

	
	CheckForPromoFrames();
	FrameManager::EnumAllFrames();
	cout << "Selecting character" << endl;
	__int64 v1 = 0;
	unsigned long characters_number = Process::ReadRel<unsigned>(WowOffsets2::Client::CharacterNumber);
	for (unsigned i = 0; i < characters_number; i++)
	{
		*((int*)&v1 + 1) = Process::ReadRel<unsigned>(WowOffsets2::Client::CharacterOffset) + 0x1d8 * i;
		wstring nm = Process::ReadString_UTF8((v1 + 0x1000000000i64) >> 32, 0);
		if (name == nm)
		{
			string frame_name = "CharSelectCharacterButton" + to_string(i + 1);
			Frame * frame = FrameManager::FindFrameByName(frame_name.c_str());
			frame->MoveMouseToFrameAndClick(1000);
			Process::PushKeyboardButton(KeyboardButton::ENTER);
			return true;
		}

	}
	cout << "Character selecting failed" << endl;
	return false;


}
bool GameInteractor::SelectCharacter(wstring & name)
{
	FrameManager::EnumAllFrames();
	vector<string> v;
	for (int i = 1; i <= 12; i++)
	{
		string button_frame_name = "CharSelectCharacterButton" + to_string(i) + "ButtonText";
		Frame * fr = FrameManager::FindFrameByName(button_frame_name);
		
		for (auto & r : fr->GetFontStrings())
		{
			v.push_back(r->GetText());
		}
		

	}
	return true;

}
bool GameInteractor::IsInWorld()
{
	unsigned result;
	result = Process::ReadRel<unsigned>(WowOffsets::Client::InWorld);
	if (result == 1)
	{
		return true;
	}
	return false;
}
bool GameInteractor::isWorldLoading()
{
	unsigned result = Process::ReadRel<unsigned>(WowOffsets::Client::InWorld);
	if (result == 256)
	{
		return true;
	}
	return false;
}
bool GameInteractor::Start(GameStartParam * param)
{
	bool loading_world = false;
	cout << "Starting game" << endl;
	StartClient();
	int c = 0;
	bool no_login_error_messages = true;
	while (1)
	{
		if (!Process::ReadRel<bool>(WowOffsets2::Client::Connecting) && !Process::ReadRel<bool>(WowOffsets2::Client::CharSelecting))
		{
			if (no_login_error_messages);
				
			else;

			Login(param->login, param->password);
			Sleep(1);
			if (!WaitWhileConnecting())
				no_login_error_messages = false;

		}
		Sleep(100);
		wstring name = L"Testorc";
		SelectCharacter(name);
		Sleep(10000000);
		/*
		if (IsInWorld())
		{
			while (isWorldLoading())
			{
				Sleep(10);
			}
			loading_world = false;
			cout << "Already in world" << endl;
			return true;
		}
		if (!loading_world)
			if (IsCharacterSelecting())
			{
				if (!SelectCharacter(param->char_name))
				{
					return false;
				}
				loading_world = true;
				continue;
			}
		if (!loading_world)
			if (IsLoggingIn())
			{
				if (!Login(param->login, param->password))
				{
					return false;
				}
				continue;
			}
		*/
	}
}
void GameInteractor::Close()
{

}
