#pragma once
#include "QuestManager.h"
#include "ObjectiveArea.h"

using namespace std;
struct _Quest;

struct QuestObjective
{
public:
	enum ObjectiveType
	{
		KillCreatures
	};
	ObjectiveType type;
};
struct QuestAreaObjective:public QuestObjective
{
public:
	ObjectiveArea area;
	QuestAreaObjective() {};
	~QuestAreaObjective() {};

};
struct QuestKillObjective:public QuestAreaObjective
{
public:
	unsigned unit_id=0;
	unsigned count=0;
	QuestKillObjective()
	{
		type = KillCreatures;
	}
	
};

class Quest
{
	string title;
public:
	unsigned id;
	unsigned objectives_count = 0;
	shared_ptr<QuestKillObjective> kill_objective1;
	QuestKillObjective kill_objective2;
	QuestKillObjective kill_objective3;
	QuestKillObjective kill_objective4;
	inline string GetTitle() { return title; }
	void GetObjectivesBlobs();
	bool Init(unsigned id);

	Quest();
	Quest(_Quest q);
	Quest(unsigned id);
	~Quest();

};