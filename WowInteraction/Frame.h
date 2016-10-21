#pragma once
using namespace Tools;
using namespace std;
class Region
{
	unsigned base;
	string text;
public:
	Region(unsigned base);
	string & GetText(bool refresh=false);

};

class Frame
{
	unsigned base=0;
	string name;
	string label_text;
	Frame * parent;
	float bottom;
	float top;
	float left;
	float right;
	unsigned id;
	vector<Frame*> children;
	vector<Region> regions;
public:
	Frame();
	Frame(unsigned base);
	~Frame(void);
	string & GetName(bool refresh = false);
	string & GetLabelText(bool refresh = false);
	void SetParent(Frame * parent);
	unsigned GetBase();
	float GetBottom(bool refresh = false);
	float GetTop(bool refresh = false);
	float GetLeft(bool refresh = false);
	float GetRight(bool refresh = false);
	inline Frame * GetParent();
	void MoveMouseToFrame();
	bool MoveMouseToFrameAndClick(unsigned delay = 0, MouseButton button=MouseButton::LEFT);
	bool IsVisible();
	bool WaitForFrameVisibility(unsigned long time = 0);
	vector<Frame*> & GetChildren();
	void PushToFrame();
	unsigned GetID();
	vector<Region> & GetRegions();

};

