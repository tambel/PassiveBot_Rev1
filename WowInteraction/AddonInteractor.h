#pragma once
#include "Tools\Tools.h"
#include "FrameManager.h"



#define LINK_ADDRESS 18923746.923874 
#define STATUS_LINK_ADDRESS 921874.9238276
#define RESULT_LINK_ADDRESS 921874.8276923
#define COMMAND_LINK_ADDRESS 874921.8276923



using namespace Tools;

class AddonError :runtime_error
{
public:
	AddonError(string message) :runtime_error(message) {}
};

namespace Addon
{
	enum Command
	{
		Logout = (unsigned)1,
		PlayerName=2,
		CurrentInteractionQuests=3,
		WaitingForEvent_GOSSIP_SHOW =100,
		WaitingForEvent_QUEST_DETAIL = 101

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



class AddonInteractor
{
	static unsigned status_address;
	static unsigned result_address;
	static unsigned command_address;
	static unsigned Read(unsigned delay=100);
	static bool Write(unsigned value);
	static bool Write(double value);
	static bool Write(Command command) { return Write((unsigned)command); }
public:
	AddonInteractor();
	~AddonInteractor();
	static bool Inject();
	static bool Logout();
	static wstring GetPlayerName();
	static void GetCurrentInteractionQuestInfo();
	static vector<GossipQuestInfo> GetCurrentInteractionQuests();

	
};

