#pragma once
#include "Quest.h"
struct _KillCreatureObjective
{
	unsigned creature_id;
	unsigned creatures_count;
};

struct _Quest
{
	unsigned id;
	unsigned f[25];
	char Name[512];
	char ObjectiveText[3000];
	char Description[6100];
	char EndText[2048];
	unsigned f2[6];
	unsigned Objective1;
	unsigned  f3[4000];
};



class QuestManager
{
	static vector<unsigned> quest_ids;
	
public:
	QuestManager();
	~QuestManager();
	static unsigned GetActiveQuestNumber();
	static unsigned GetQuestJournalIndex(unsigned id);
	static void EnumActiveQuests();
	static vector<unsigned> & GetQuestIds() { return quest_ids; }
	static Quest  GetQuest(unsigned id);
	static unsigned GetQuestBlob(unsigned id);
	//static GetQuestObjectivesArea();

	static void SelectQuestInJournal(unsigned id); 
	



};

