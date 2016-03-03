#pragma once


struct Quest
{
	unsigned id;
	char f[128-0x1c];
	char Name[512];
	char ObjectiveText[3000];
	char Description[3000];
	char EndText[512];
};
struct Questt
{
	DWORD Id;
	DWORD Method;
	DWORD Level;
	DWORD RequiredLevel;
	DWORD AreaIdOrSortId;
	DWORD dword14;
	DWORD QuestInfoId;
	DWORD SuggestedPlayers;
	DWORD FriendlyFactionId;
	DWORD FriendlyFactionAmount;
	DWORD RewardMoney;
	DWORD RewardMoneyInsteadOfXP;
	DWORD RewardSpellId;
	DWORD RewardSpellId2;
	DWORD RewardHonor;
	float RewardHonorBonus;
	DWORD StartingItemId;
	DWORD Flags;
	DWORD Flags2;
	DWORD RewardItem[4];
	DWORD RewardItemCount[4];
	DWORD dword6C;
	DWORD RewardChoiceItem[6];
	DWORD RewardChoiceItemCount[6];
	DWORD dwordA0;
	DWORD dwordA4;
	DWORD dwordA8;
	DWORD dwordAC;
	DWORD dwordB0;
	int dwordB4;
	DWORD PointMapId;
	float PointX;
	float PointY;
	//DWORD PointOptional;
	char Name[512];
	char ObjectiveText[3000];
	char Description[3000];
	char EndText[512];
	DWORD ObjectiveId[4];
	DWORD ObjectiveRequiredCount[4];
	DWORD RewardTitleId;
	DWORD RewardTalentPoints;
	DWORD dword1C60;
	DWORD RewardSkillId;
	DWORD RewardSkillPoints;
	DWORD PortraitGiverId;
	DWORD PortraitTurnInId;
	char PortraitGiverText[1024];
	char PortraitGiverName[256];
	char PortraitTurnInDescription[1024];
	char PortraitTurnInName[256];
	char OverrideObjectiveText[2048];
	DWORD RewardReputationFaction[5];
	DWORD FactionRewardID[5];
	DWORD RewardReputationOverride[5];
	DWORD dword2EB0;
	DWORD RewardCurrencies[4];
	DWORD RewardCurrencyCount[4];
	DWORD dword2ED4;
	DWORD dword2ED8;
	//QuestObjectivesInfo ObjectivesInfo;
};

struct AQuest
{
	DWORD Id;
	DWORD Method;
	DWORD Level;
	DWORD RequiredLevel;
	DWORD AreaIdOrSortId;
	DWORD dword14;
	DWORD QuestInfoId;
	DWORD SuggestedPlayers;
	DWORD FriendlyFactionId;
	DWORD FriendlyFactionAmount;
	DWORD RewardMoney;
	DWORD RewardMoneyInsteadOfXP;
	DWORD RewardSpellId;
	DWORD RewardSpellId2;
	DWORD RewardHonor;
	float RewardHonorBonus;
	DWORD StartingItemId;
	DWORD Flags;
	DWORD Flags2;
	DWORD RewardItem[4];
	DWORD RewardItemCount[4];
	DWORD dword6C;
	DWORD RewardChoiceItem[6];
	DWORD RewardChoiceItemCount[6];
	DWORD dwordA0;
	DWORD dwordA4;
	DWORD dwordA8;
	DWORD dwordAC;
	DWORD dwordB0;
	int dwordB4;
	DWORD PointMapId;
	float PointX;
	float PointY;
	DWORD PointOptional;
	char Name[512];
	char ObjectiveText[3000];
	char Description[3000];
	char EndText[512];
	DWORD ObjectiveId[4];
	DWORD ObjectiveRequiredCount[4];
	DWORD RewardTitleId;
	DWORD RewardTalentPoints;
	DWORD dword1C60;
	DWORD RewardSkillId;
	DWORD RewardSkillPoints;
	DWORD PortraitGiverId;
	DWORD PortraitTurnInId;
	char PortraitGiverText[1024];
	char PortraitGiverName[256];
	char PortraitTurnInDescription[1024];
	char PortraitTurnInName[256];
	char OverrideObjectiveText[2048];
	DWORD RewardReputationFaction[5];
	DWORD FactionRewardID[5];
	DWORD RewardReputationOverride[5];
	DWORD dword2EB0;
	DWORD RewardCurrencies[4];
	DWORD RewardCurrencyCount[4];
	DWORD dword2ED4;
	DWORD dword2ED8;
	//QuestObjectivesInfo ObjectivesInfo;
};


class QuestManager
{
	static vector<unsigned> quest_ids;
public:
	QuestManager();
	~QuestManager();
	static unsigned GetActiveQuestNumber();
	static void EnumActiveQuests();
	static vector<unsigned> & GetQuestIds() { return quest_ids; }
	static Quest  GetQuest(unsigned id);


};

