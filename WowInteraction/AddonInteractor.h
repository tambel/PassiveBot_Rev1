#pragma once
#include "Tools\Tools.h"
#include "FrameManager.h"
#include "GameManager.h"



#define LINK_ADDRESS 18923746.923874 
#define STATUS_LINK_ADDRESS 921874.9238276
#define RESULT_LINK_ADDRESS 921874.8276923
#define COMMAND_LINK_ADDRESS 874921.8276923
#define EVENT_LINK_ADDRESS 47321.8764545


using wstring_list = vector<wstring>;


using namespace Tools;

class AddonError :runtime_error
{
public:
	AddonError(string message) :runtime_error("Addon error: "+message) {}
};

namespace Addon
{
	enum Command
	{
		Logout = (unsigned)1,
		PlayerName=2,
		CurrentInteractionQuests=3,
		CLEAR_EVENTS =100,
		WaitingForEvent_QUEST_DETAIL = 101,
		INJECTED=1000

	};
	enum Status
	{
		SUCCESS=1,
		IN_PROCESS=2
	};
}

using namespace Addon;

enum Response
{
	LogoutContdown=0,
	LogoutImmediate = 1
};


struct GossipQuestInfo
{
	unsigned id;
	wstring title;
	
};

struct  TargerQuestGiverInteractionResult
{
	vector<GossipQuestInfo> infos;
	bool quest_detail_triggered = false;
};

struct SelectedGossipQuestInfo
{
	unsigned id;
	wstring title;
};


class AddonInteractor
{
	static unsigned status_address;
	static unsigned result_address;
	static unsigned command_address;
	static unsigned event_address;
	static unsigned command_string_address;
	static const  unsigned command_string_size = 1024;
	static unsigned result_string_address;

	unsigned Read(unsigned delay);
	static unsigned ReadByProgressStatus(unsigned delay=100);
	static void  Write(unsigned value);
	static void Write(double value);
	static void Write(Command command) { Write((unsigned)command); }
	static void WriteStatus(unsigned value);
	static void WriteCommandString(const string & str);
	static string ReadResultString();
	static unsigned ExecuteRegularCommand(Command command);
	static wstring ReadEvents();
public:
	AddonInteractor();
	~AddonInteractor();
	static bool Inject2(bool manual_confirm=false);
	static bool Inject(bool manual_confirm = false);
	static bool Inject_FindString();
	static bool Logout();
	static wstring GetPlayerName();
	static vector<wstring> WaitForEvents(vector<wstring> & event_names, bool infinite=false);
	static void ClearEvents();




	static void GetCurrentInteractionQuestInfo();

	static TargerQuestGiverInteractionResult GetCurrentInteractionQuests();
	static SelectedGossipQuestInfo GetSelectedQuest();

	//StringCommands

	static string ExecuteCommand(const string & cmd, bool manual_confirm=false);
	static void ConfirmCommandExecution();
	static string WaitForResult(int attempts_count = 10, int delay = 500);

	static void ExecuteLuaCode(const string &str);
	static void WriteCommand(const string & cmd);


	
};

