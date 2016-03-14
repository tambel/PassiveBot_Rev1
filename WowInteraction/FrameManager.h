#pragma once
#include "Frame.h"
using namespace std;

class FrameManager
{
	static vector <Frame*>  frames;
	static float screen_width;
	static float screen_heigth;
	
public:
	FrameManager(void);
	~FrameManager(void);
	static vector <Frame*> & GetFrames();
	static void EnumAllFrames();
	static void InitKnownFrames();
	static void FindParents();
	static void DumpAllFramesNames();
	static void ClearFrames();
	static float GetScreenWidth(bool refresh = false);
	static float GetScreenHeigth(bool refresh = false);
	static Frame * FindFrameByName(const string & name);
	static Frame * FindFrameByAddress(unsigned address);
	static vector<Frame*> FindAllChildren(Frame * frame);
	static Frame QuestScrollFrame;
	static Frame AllQuestButtonFrame;
	static Frame QuestListFrame;


};

