#include "stdafx.h"
using namespace Tools;
namespace Wow
{

	GameInteractor::GameInteractor(void)
	{
	}
	GameInteractor::~GameInteractor(void)
	{
	}
	bool GameInteractor::Login(string & login, string  & password)
	{
		cout<<"Logging in"<<endl;
		FrameManager::EnumAllFrames();
		Frame * email_frame= FrameManager::FindFrameByName("AccountLoginAccountEdit");
		if (!email_frame)
		{
			cout<<"Email frame not found";
			return false;
		}

		if (!email_frame->WaitForFrameVisibility() || !email_frame->IsVisible())
		{
			cout<<"Email frame is not visible"<<endl;
			return false;
		}
		email_frame->MoveMoseToFrame();
		Process::MouseClick(MouseButton::LEFT);
		Process::MultipleKeyboardButtonPush(KeyboardButton::ARROW_RIGHT,20,50);
		Process::MultipleKeyboardButtonPush(KeyboardButton::BACKSPACE,40,40);
		Process::SetLanguage(Language::ENGLISH);
		Process::TypeByKeyboard(login);
		Frame * password_frame= FrameManager::FindFrameByName("AccountLoginPasswordEdit");
		password_frame->MoveMoseToFrame();
		Process::MouseClick(MouseButton::LEFT);
		Process::TypeByKeyboard(password);
		Process::PressKeyboardButton(KeyboardButton::ENTER);
		WaitForAuthentification();
		
		return true;
	}
	bool GameInteractor::StartClient()
	{
		unsigned attempts_count=10;
		if (!Process::FindExistingProcess())
		{
			cout<<"Launching new game process"<<endl;
			Process::LaunchProcess();
			Sleep(20000);
		}
		cout<<"Game process found"<<endl;
		unsigned attempts=0;
		while(!Process::Init() && attempts<attempts_count)
		{
			cout<<"Retry"<<endl;
			Sleep(5000);
			attempts++;
		}
		if (attempts==attempts_count)
		{
			cout<<"Attempts limit is reached"<<endl;
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
		return Process::ReadRel<char>(WowOffsets::Client::Loaded); 
	}
	bool GameInteractor::IsCharacterSelecting()
	{
		FrameManager::EnumAllFrames();
		Frame * char_select=FrameManager::FindFrameByName("CharSelectCharacterButton1");
		
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
	bool GameInteractor::IsLoggingIn()
	{
		if (isWorldLoading())
			return false;
		FrameManager::EnumAllFrames();
		Frame * email_frame= FrameManager::FindFrameByName("AccountLoginAccountEdit");
		if (!email_frame)
		{
			return false;
		}
		if (!email_frame->IsVisible())
		{
			return false;
		}
		if (!IsCharacterSelecting() && !IsInWorld() && IsLoaded())
		{
			return true;
		}
		return false;
	}
	bool GameInteractor::WaitUntilClientLoad()
	{
		cout<<"Waiting for client load..."<<endl;
		unsigned attempts=0;
		while(!IsLoaded() && attempts<6000)
		{
			Sleep(10);
			attempts++;
		}
		if (IsLoaded())
		{
			cout<<"Done"<<endl;
			return true;
		}
		else
		{
			cout<<"Client not loaded"<<endl;
			return false;
		}

	}
	bool GameInteractor::WaitForAuthentification()
	{
		cout<<"Waiting for client authentification..."<<endl;
		unsigned attempts=0;
		while(Process::ReadRel<char>(WowOffsets::Client::Unavalible) && attempts<6000 )
		{
			Sleep(10);
			attempts++;
		}
		Sleep(10000);
		if (!Process::ReadRel<char>(WowOffsets::Client::Unavalible))
		{
			cout<<"Done"<<endl;
			return true;
		}
		else
		{
			cout<<"Authentification failed"<<endl;
			return false;
		}
	}
	void GameInteractor::CheckForPromoFrames()
	{
		cout<<"Checking for promo frames"<<endl;
		FrameManager::EnumAllFrames();
		Frame *starter=FrameManager::FindFrameByName("StarterEditionPopUpExitButton");
		unsigned attempts=0;
		if (!starter)
		{
			return;
		}
		while (!starter->IsVisible() && attempts<1000)
		{
			Sleep(10);
			attempts++;
		}
		if (starter->IsVisible())
		{
			
			starter->MoveMoseToFrame();
			Sleep(5000);
			Process::MouseClick(MouseButton::LEFT,100);
			cout<<"\"StarterEditionPopUpExitButton\" frame found and closed"<<endl;
		}
		if (starter->IsVisible())
		{
			return;
		}
	}
	void GameInteractor::Test()
	{
		FrameManager::EnumAllFrames();
		vector<Frame*> * frames=FrameManager::GetFrames();
		Frame *frame1=FrameManager::FindFrameByName("CharSelectCharacterButton1");
		vector<wchar_t*> names=vector<wchar_t*>();
		vector<unsigned> u=vector<unsigned>();
		vector<unsigned> u2=vector<unsigned>();
		for (unsigned i=0;i<1000;i+=4)
		{
			u.push_back(Process::Read<unsigned>(frame1->GetBase()+i));
		}
		Sleep(7000);
		for (unsigned i=0;i<1000;i+=4)
		{
			u2.push_back(Process::Read<unsigned>(frame1->GetBase()+i));
		}
		Frame *frame2=FrameManager::FindFrameByName("CharSelectAccountUpgradePanelFeature3");
		Frame *frame3=FrameManager::FindFrameByName("StarterEditionPopUpFeature2");
		Frame *frame4=FrameManager::FindFrameByName("StarterEditionPopUpFeature3");
		Frame *frame5=FrameManager::FindFrameByName("StarterEditionPopUpExitButton");
		Sleep(5000);
		frame1->MoveMoseToFrame();
		bool y=frame5->IsVisible();

	}
	bool GameInteractor::SelectCharacter(wstring & name)
	{
		CheckForPromoFrames();
		FrameManager::EnumAllFrames();
		cout<<"Selecting character"<<endl;
		__int64 v1=0;
		unsigned long characters_number=Process::ReadRel<unsigned>(WowOffsets::Client::CharactersNumber);
		for (int i=0;i<characters_number;i++)
		{
			*((int*)&v1+1)=Process::ReadRel<unsigned>(WowOffsets::Client::CharactersOffset)+464*i;
			wstring nm=Process::ReadString_UTF8((v1 + 0x1000000000i64) >> 32,0);
			if (name==nm)
			{
				string frame_name="CharSelectCharacterButton"+to_string(i+1);
				Frame * frame =FrameManager::FindFrameByName(frame_name.c_str());
				frame->MoveMoseToFrame();
				Process::DoubleClick(MouseButton::LEFT);
				return true;
			}

		}
		cout<<"Character selecting failed"<<endl;
		return false;


	}
	bool GameInteractor::IsInWorld()
	{
		unsigned result;
		result= Process::ReadRel<unsigned>(WowOffsets::Client::InWorld);
		if (result==1)
		{
			return true;
		}
		return false;
	}
	bool GameInteractor::isWorldLoading()
	{
		unsigned result=Process::ReadRel<unsigned>(WowOffsets::Client::InWorld);
		if (result==256)
		{
			return true;
		}
		return false;
	}
	bool GameInteractor::Start(GameStartParam * param)
	{
		bool loading_world = false;
		cout<<"Starting game"<<endl;
		StartClient();
		int c=0;
		while (1)
		{
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
			Sleep(100);
		}
	}
	void GameInteractor::Close()
	{

	}
}