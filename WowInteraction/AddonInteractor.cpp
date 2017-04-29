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
unsigned AddonInteractor::command_string_address = 0;
unsigned AddonInteractor::result_string_address = 0;
Region * AddonInteractor::result__fontstring = nullptr;

OutComString AddonInteractor::command_string;
OutComString AddonInteractor::flags_string;
map<string, unsigned> AddonInteractor::flags;


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

unsigned AddonInteractor::ReadByProgressStatus(unsigned delay)
{
	return 0;
}

void AddonInteractor::Write(unsigned value)
{
}

void AddonInteractor::Write(double value)
{
}


void AddonInteractor::WriteStatus(unsigned value)
{
}

void AddonInteractor::WriteCommandString(const string & str)
{
	auto pad_string = [](const string & str, unsigned length)
	{
		if (str.length() < length)
		{
			unsigned pad_size = length - str.length();
			char * pcstr = new char[pad_size];
			memset(pcstr, '&', pad_size);
			return str + string(pcstr, pad_size);

		}
	};
	unsigned str_length = str.length();
	if (str_length <= command_string_size)
	{

		string padded = pad_string(to_string(str_length) + ";" + str + "[!]", command_string_size);
		Process::WriteRaw(const_cast<char*>(padded.c_str()), padded.length(), command_string_address);
	}

}

string AddonInteractor::ReadResultString()
{
	
	auto read_com_string = [](string & str)
	{
		unsigned length = str.length();
		int i = 0;
		string ns1="",ns2 = "";
		int n1, n2;
		while (str[i] != ';' && str[length - i - 1] != ';' && i<length)
		{
			ns1 = ns1 + str[i];
			ns2 = str[length - i - 1] + ns2;
			i++;
		}
		n1 = stoi(ns1);
		n2 = stoi(ns2);
		if (n1 == n2)
		{
			string result = str.substr(i+1, n1);
			if (result.length() == n1)
			{
				return result;
			}
		}
		return string("WrongStringFromAddon");
	};
	string raw_result_string =result__fontstring->GetText(true);
	//string raw_result_string = move(Process::ReadASCII(result_string_address, 0));
	string result=move(read_com_string(raw_result_string));
	return result;

}

unsigned AddonInteractor::ExecuteRegularCommand(Command command)
{
	return 0;
}

wstring AddonInteractor::ReadEvents()
{
	return wstring();
}

AddonInteractor::AddonInteractor()
{
}


AddonInteractor::~AddonInteractor()
{
}

bool AddonInteractor::InitCommunication()
{
	return (FindCommunicationComponents() && InitFlags());
}

bool AddonInteractor::FindCommunicationComponents()
{
	FrameManager::EnumAllFrames();
	Frame *addon_main_frame = FrameManager::FindFrameByName("TestAddon_MainFrame");
	auto regions = addon_main_frame->GetRegions();
	bool command_string_found = false;
	bool result_string_found = false;


	try
	{
		for (auto region : regions)
		{

			if (region->GetType() == RegionType::FONT_STRING)
			{
				if (region->GetName() == "MagickString")
				{
					command_string = OutComString(&*region, 1024);
				}
				if (region->GetName() == "FlagsString")
				{
					flags_string = OutComString(&*region, 2);
				}

				if (region->GetName() == "ResultString")
				{

					result__fontstring = region.get();
					//result_string_address = region->GetTextAddress();//Process::Read<unsigned>(region->GetBase() + WowOffsets2::FrameManager2::FontStringRegionText);
					if (result__fontstring)
					{
						result_string_found = true;
					}

				}
			}
		}
	}
	catch (ComStringInitError & e)
	{
		throw_with_nested(AddonError("\"FindCommunicationComponents\" function failed. Cant initialize some fontstrings"));
	}
	
	return true;
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
	return Process::ReadString_UTF8(Process::Read<unsigned>	(result_address) + 20,0);
	
	 
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
void print_what(const std::runtime_error& e) {
	std::cerr << e.what() << '\n';
	try {
		std::rethrow_if_nested(e);
	}
	catch (const std::runtime_error& nested) {
		std::cerr << "nested: ";
		print_what(nested);
	}
}
TargerQuestGiverInteractionResult AddonInteractor::GetCurrentInteractionQuests()
{
	wstring_list ev = wstring_list();
	//vector<GossipQuestInfo> result = vector<GossipQuestInfo>();
	TargerQuestGiverInteractionResult result;
	try
	{
		
		ev = WaitForEvents(wstring_list({ L"GOSSIP_SHOW", L"QUEST_DETAIL" }), true);
	}
	catch (runtime_error & e)
	{
		print_what(e);
		throw_with_nested(AddonError("\"GetCurrentInteractionQuests\" function failed. Event wait failed"));
	}
	wstring_list vres = move(parse_string(ev[1]));
	if (ev[0] == L"GOSSIP_SHOW")
	{

		for (int i = 0; i < vres.size(); i += 7)
		{
			GossipQuestInfo info = GossipQuestInfo();
			info.title = vres[i];
			result.infos.push_back(info);
		}

	}
	else if (ev[0] == L"QUEST_DETAIL")
	{
		GossipQuestInfo info = GossipQuestInfo();
		info.id = stoi(vres[0]);
		info.title = vres[1];
		result.infos.push_back(info);
		result.quest_detail_triggered = true;
	}
	return result;
}

SelectedGossipQuestInfo AddonInteractor::GetSelectedQuest()
{
	wstring_list ev = wstring_list();
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

string AddonInteractor::ExecuteCommand(const string & cmd, bool manual_confirm)
{
	string result;
	WriteCommand(cmd);
	if (!manual_confirm)
		ConfirmCommandExecution();
	if (manual_confirm)
		result = move(WaitForResult(-1));
	else
		result = move(WaitForResult());
	return result;
}

void AddonInteractor::ConfirmCommandExecution()
{
	GameManager::UseHotKey(wstring(L"!1"));
}

string AddonInteractor::WaitForResult(int attempts_count, int delay)
{
	
	string result= ReadResultString();
	while ((result == "InProcess" || result == "NoTasks" || result =="WrongStringFromAddon") && attempts_count)
	{
		result = ReadResultString();
		if (attempts_count!=-1)
			attempts_count--;
		Sleep(delay);

	}
	return result;
}

void AddonInteractor::ExecuteLuaCode(const string & str)
{
	string code = str;
	WriteCommandString(code);
}

void AddonInteractor::SetFlag(string name)
{
	flags_string.WriteByte('1', flags[name]);
}

void AddonInteractor::UnsetFlags(string name)
{
	flags_string.WriteByte('0', flags[name]);
}

bool AddonInteractor::InitFlags()
{
	flags.insert(pair<string, unsigned>("confirm", 0));
	flags.insert(pair<string, unsigned>("clear", 1));
	return true;
}

bool AddonInteractor::TakeQuestMapScreenshots(int quest_id)
{
	string result=move(AddonInteractor::ExecuteCommand("TakeQuestMapScreenshots "+to_string(quest_id),true));
	return false;
}

void AddonInteractor::WriteCommand(const string & cmd)
{
	//WriteCommandString(cmd);
	command_string.Write(cmd);
}
