#include "stdafx.h"
#include "AddonInteractor.h"
#include "GameInteractor.h"
#include "ObjectManager.h"
#include  <math.h>


vector<wstring> parse_string(wstring & str)
{
	vector<wstring> result = vector<wstring>();
	wstring s = L"";
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] != L';')
			s += str[i];
		else
		{
			result.push_back(s);
			s = L"";
			if (i < str.length() - 1)
				if (str[i + 1] == L';')
					i++;
		}

	}
	if (s != L"")
		result.push_back(s);
	return result;
}




unsigned AddonInteractor::status_address = 0;
unsigned AddonInteractor::result_address = 0;
unsigned AddonInteractor::command_address = 0;


unsigned AddonInteractor::Read(unsigned delay)
{
	unsigned result;
	unsigned status;
	char attempts = 3;
	do
	{
		Sleep(delay);
		status = Process::Read<double>(status_address);
		if (status)
		{
			result=Process::Read<double>(result_address);
		}
		attempts--;
	} while (!status && attempts);

	if (!status)
		throw AddonError("Addon not responding");

	return result;
}

bool AddonInteractor::Write(unsigned value)
{
	return Write(static_cast<double>(value));
}

bool AddonInteractor::Write(double value)
{
	return Process::Write<double>(value, command_address);
	Sleep(100);
}

AddonInteractor::AddonInteractor()
{
}


AddonInteractor::~AddonInteractor()
{
}

bool AddonInteractor::Inject()
{
	status_address = 0;
	result_address = 0;
	command_address = 0;
	vector<unsigned> found = Process::FindInMemory(LINK_ADDRESS);
	Frame * button = nullptr;
	unsigned char attempts = 3;
	while (attempts && !button)
	{
		button = FrameManager::FindFrameByName("MagickButton", true);
		attempts--;
	}
	if (!button)
		return false;
	button->MoveMouseToFrameAndClick(100);

	
	attempts = 3;
	while (attempts && (!status_address || !result_address))
	{
		Sleep(1000);
		for (auto f : found)
		{
			if (Process::Read<double>(f) == STATUS_LINK_ADDRESS && !status_address)
			{
				status_address = f;
			}
			if (Process::Read<double>(f) == RESULT_LINK_ADDRESS && !result_address)
			{
				result_address = f;
			}
			if (Process::Read<double>(f) == COMMAND_LINK_ADDRESS && !command_address)
			{
				command_address = f;
			}
			
		}
		attempts--;
	}

	return status_address && result_address && command_address;
}

bool AddonInteractor::Logout()
{
	Write((unsigned)Command::Logout);
	Sleep(500);
	unsigned countdown = 20;
	while (countdown && GameInteractor::IsInWorld() )
	{
		Sleep(1000);
		countdown--;
	}
	Sleep(5000);
	return !GameInteractor::IsInWorld();
}

wstring AddonInteractor::GetPlayerName()
{
	Write((unsigned)Command::PlayerName);
	Sleep(100);
	return Process::ReadString_UTF8(Process::Read<unsigned>(result_address) + 20,0);
	
	 
}

void AddonInteractor::GetCurrentInteractionQuestInfo()
{
	Write(Command::WaitingForEvent_QUEST_DETAIL);
	Sleep(100);
	unsigned result;
	do
	{
		Sleep(100);
		result = Read();
	} while (result == 2);

	unsigned id = Process::Read<double>(Process::Read<unsigned>(Process::Read<unsigned>(result_address)+0x14));
	wstring n = Process::ReadString_UTF8(Process::Read<unsigned>(Process::Read<unsigned>(Process::Read<unsigned>(result_address) + 0x14) + 0x28) + 0x14,0);


}

vector<GossipQuestInfo> AddonInteractor::GetCurrentInteractionQuests()
{
	Write(Command::WaitingForEvent_GOSSIP_SHOW);
	Sleep(100);
	unsigned result;
	do
	{
		result = Read();
	} while (result == 2);


	wstring res = Process::ReadString_UTF8(Process::Read<unsigned>(result_address) + 0x14,0);
	vector<wstring> vres = parse_string(res);
	int type = stoi(vres[0]);
	vector<GossipQuestInfo> infos = vector<GossipQuestInfo>();
	if (type==1)
	{
		GossipQuestInfo info = GossipQuestInfo();
		info.id = stoi(vres[1]);
		info.title = vres[2];
		infos.push_back(info);
	}
	if (type == 2)
	{
		for (int i = 1; i < vres.size(); i+=7)
		{
			GossipQuestInfo info = GossipQuestInfo();
			info.title = vres[i];
			infos.push_back(info);
		}
	}

	return infos;
}
