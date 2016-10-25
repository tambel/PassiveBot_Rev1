#include "stdafx.h"
#include "GameInteractor.h"
#include <codecvt>
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
	

	Frame * email_frame = nullptr;
	Frame * password_frame = nullptr;
	Frame * f1 = new Frame(Process::ReadRel<unsigned>(WowOffsets2::FrameManager2::CurrentKeyBoardFocusFrame));
	Sleep(1000);
	Process::PushKeyboardButton(KeyboardButton::TAB);
	Sleep(1000);
	Frame * f2 = new Frame(Process::ReadRel<unsigned>(WowOffsets2::FrameManager2::CurrentKeyBoardFocusFrame));
	if (f1->GetTop() < f2->GetTop())
	{
		email_frame = f2;
		password_frame = f1;
	}
	else
	{
		email_frame = f1;
		password_frame = f2;
	}
	auto enter_text_and_push=[](Frame * frame, string text)
	{
		frame->MoveMouseToFrameAndClick();
		Process::MultipleKeyboardButtonPush(KeyboardButton::ARROW_RIGHT, 20, 50);
		Sleep(1000);
		Process::MultipleKeyboardButtonPush(KeyboardButton::BACKSPACE, 40, 40);
		Sleep(1000);
		Process::SetLanguage(Language::ENGLISH);
		Sleep(1000);
		Process::TypeByKeyboard(text);
		Sleep(1000);
		Process::PushKeyboardButton(KeyboardButton::TAB);
		Sleep(1000);
	};
	enter_text_and_push(email_frame,login);
	enter_text_and_push(password_frame, password);
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
bool GameInteractor::SelectCharacter(wstring & character_name)
{
	FrameManager::EnumAllFrames();
	vector<string> v;
	string name;
	Frame * character_frame = nullptr;
	for (int i = 1; i <= 12; i++)
	{
		string button_frame_name = "CharSelectCharacterButton" + to_string(i) + "ButtonText";
		string button_frame_region_charname = "CharSelectCharacterButton" + to_string(i) + "ButtonTextName";
		Frame * fr = FrameManager::FindFrameByName(button_frame_name);
		
		for (auto & r : fr->GetFontStrings())
		{
			name = r->GetName();
			if (name == button_frame_region_charname)
			{
				if (r->GetWText() == character_name)
				{
					character_frame = fr;
					break;
				}
			}
		}
		if (character_frame)
			break;
	}
	if (!character_frame)
	{
	}
	character_frame->MoveMouseToFrameAndClick(1000);
	Process::PushKeyboardButton(KeyboardButton::ENTER);
	return true;

}
bool GameInteractor::IsInWorld()
{
	try
	{
		return Process::ReadRel<bool>(WowOffsets2::Client::InWorld);
	}
	catch (MemoryReadException e)
	{
		return false;
	}
	
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
			if (no_login_error_messages)
				Login(param->login, param->password);
			else;
				
			
			if (!WaitWhileConnecting())
			{
				no_login_error_messages = true;
			}
			else
			{
				Sleep(5000);
				no_login_error_messages = false;
			}
		}
		else if (Process::ReadRel<bool>(WowOffsets2::Client::Connecting) && Process::ReadRel<bool>(WowOffsets2::Client::CharSelecting))
		{
			SelectCharacter(param->char_name);
			Sleep(5000);
		}
		else if (Process::ReadRel<bool>(WowOffsets2::Client::InWorld))
		{
			Sleep(5000);
			cout << "InWorld" << endl;
			int result=param->working_func();
		}
		Sleep(100);
	}

}
void GameInteractor::Close()
{
	FrameManager::ClearFrames();
}
