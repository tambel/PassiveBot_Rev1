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
unsigned AddonInteractor::event_address = 0;

unsigned AddonInteractor::ReadByProgressStatus(unsigned delay)
{
	unsigned result;
	unsigned status;
	char attempts = 10;
	try
	{
		do
		{
			Sleep(delay);
			status = Process::Read<double>(status_address);
			if (status)
			{
				result = Process::Read<double>(result_address);
			}
			attempts--;
		} while (status == Status::IN_PROCESS && attempts);
	}
	catch (MemoryReadException & e)
	{
		throw_with_nested(AddonError("ReadByProgressStatus failed. Cant read memory"));
	}
	if (!status || !attempts)
		throw AddonError("ReadByProgressStatus failed. Status not changing");
	return result;
}

void AddonInteractor::Write(unsigned value)
{
	return Write(static_cast<double>(value));
}

void AddonInteractor::Write(double value)
{
		Process::Write<double>(value, command_address);
		Sleep(100);
}

void  AddonInteractor::WriteStatus(unsigned value)
{
	Process::Write<double>(static_cast<double>(value), status_address);
}

unsigned AddonInteractor::ExecuteRegularCommand(Command command)
{
	try
	{
		WriteStatus(Status::IN_PROCESS);
		Write(command);
		return ReadByProgressStatus();
	}
	catch (runtime_error & e)
	{
		throw_with_nested(AddonError("ExecuteRegularCommand function failed"));
	}
}

wstring AddonInteractor::ReadEvents()
{
	try
	{
		return Process::ReadString_UTF8(Process::Read<unsigned>(event_address) + 20, 0);
	}
	catch (MemoryReadException & e)
	{
		throw_with_nested(AddonError("\"ReadEvents\" function failed. Cant read string"));
	}
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
	while (attempts && (!status_address || !result_address || !command_address || !event_address))
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
			if (Process::Read<double>(f) == EVENT_LINK_ADDRESS && !event_address)
			{
				event_address = f;
			}

		}
		attempts--;
	}

	if (status_address && result_address && command_address && event_address)
	{
		try
		{
			ExecuteRegularCommand(Command::INJECTED);
			return true;
		}
		catch (AddonError & e)
		{
			throw_with_nested(AddonError("Cant inject to addon. \"INJECTED\" command execution failed."));
		}
	}
	else
	{

	}
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

vector<wstring> AddonInteractor::WaitForEvents(vector<wstring>& event_names, bool infinite)
{
	bool in_brackets = false;
	unsigned position = 0;
	unsigned name_start = 0;
	unsigned name_end = 0;
	unsigned data_start;
	unsigned data_end;
	wstring events_string = L"";
	vector<wstring> result = vector<wstring>();
	unsigned max_wait = 10;
	unsigned count = 0;
	try
	{
		ClearEvents();
		do
		{
			Sleep(500);
			result.clear();
			do
			{
				Sleep(50);
				events_string = move(ReadEvents());
			} while (events_string.length() == 0 || events_string ==L"None");
			wstring event_name = L"";
			wstring event_data = L"";
			const wchar_t * str = events_string.c_str();
			for (int i = 0; i < events_string.length(); i++)
			{
				if (i == 0 || (i > 0 && str[i - 1] == ']'))
					name_start = i;
				if (str[i] == '[')
				{
					name_end = i - 1;
					data_start = i + 1;
				}
				if (str[i] == ']')
				{
					data_end = i - 1;
					event_name = events_string.substr(name_start, name_end - name_start+1);
					event_data = events_string.substr(data_start, data_end - data_start+1);
					for (auto es : event_names)
					{
						if (es == event_name)
						{
							result.push_back(event_name);
							result.push_back(event_data);
							return result;
						}
					}
				}
			}
			count++;
		} while (count<max_wait || infinite);
		throw(AddonError("\"WiatForEvents\" function failed. Wait time left"));
	}
	catch (AddonError & e)
	{
		throw_with_nested(AddonError("\"WiatForEvents\" function failed. Cant read events"));
	}
}

void AddonInteractor::ClearEvents()
{
	try
	{
		ExecuteRegularCommand(Command::CLEAR_EVENTS);
	}
	catch (AddonError & e)
	{
		throw_with_nested(AddonError("ClearEvents error"));
	}
}

void AddonInteractor::GetCurrentInteractionQuestInfo()
{
	/*
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
	*/

}

vector<GossipQuestInfo> AddonInteractor::GetCurrentInteractionQuests()
{
	wstring_list ev = wstring_list();
	vector<GossipQuestInfo> result = vector<GossipQuestInfo>();
	try
	{
		ev = WaitForEvents(wstring_list({ L"GOSSIP_SHOW", L"QUEST_DETAIL" }), true);
	}
	catch (AddonError & e)
	{
		throw_with_nested(AddonError("\"GetCurrentInteractionQuests\" function failed. Event wait failed"));
	}
	wstring_list vres = move(parse_string(ev[1]));
	if (ev[0] == L"GOSSIP_SHOW")
	{

		for (int i = 0; i < vres.size(); i += 7)
		{
			GossipQuestInfo info = GossipQuestInfo();
			info.title = vres[i];
			result.push_back(info);
		}

	}
	else if (ev[0] == L"QUEST_DETAIL")
	{
		GossipQuestInfo info = GossipQuestInfo();
		info.id = stoi(vres[1]);
		info.title = vres[2];
		result.push_back(info);
	}
	return result;
}

SelectedGossipQuestInfo AddonInteractor::GetSelectedQuest()
{
	wstring_list ev = wstring_list();
	vector<GossipQuestInfo> result = vector<GossipQuestInfo>();
	try
	{
		ev = WaitForEvents(wstring_list({L"QUEST_DETAIL" }), true);
	}
	catch (AddonError & e)
	{
		throw_with_nested(AddonError("\"GetSelectedQuest\" function failed. Event wait failed"));
	}
	if (ev[0] == L"QUEST_DETAIL")
	{
		wstring_list vres = move(parse_string(ev[1]));
		SelectedGossipQuestInfo info;
		info.id = stoi(vres[0]);
		info.title = vres[1];
		return info;
	}
	throw AddonError("\"GetSelectedQuest\" function failed. Wrong result string");
}
