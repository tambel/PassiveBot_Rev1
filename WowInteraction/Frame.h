#pragma once
#include "FrameManager.h"
#include "Tools\Memory.h"
using namespace Tools;
using namespace std;

class Frame;

enum RegionType {
	FONT_STRING,
	TEXTURE,
	NONE
};
class Region
{
	
	unsigned base;
	wstring wtext;
	string text;
	RegionType type;
	string name;

	Frame * parent;
public:
	Region(unsigned base, Frame * parent);
	inline unsigned GetTextAddress();
	wstring & GetWText(bool refresh=false);
	string & GetText(bool refresh = false);
	RegionType GetType();
	string & GetName(bool refresh = false);
	Frame * GetParent() { return this->parent; }
	inline unsigned GetBase() { return base; }
	


};

class Frame
{
	static int const_screen_width;
	static int const_screen_height;
	unsigned base=0;
	string name;
	string label_text;
	Frame * parent;
	float bottom;
	float top;
	float left;
	float right;

	float bottom_prop;
	float top_prop;
	float left_prop;
	float right_prop;
	float width;
	float height;
	unsigned id;
	vector<Frame*> children;
	vector<shared_ptr<Region>> regions;
	vector<shared_ptr<Region>> font_strings;
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
	void RefreshSize();
	float GetBottomProp();
	float GetTopProp();
	float GetLeftProp();
	float GetRightProp();
	inline Frame * GetParent();
	void MoveMouseToFrame();
	bool MoveMouseToFrameAndClick(unsigned delay = 0, MouseButton button=MouseButton::LEFT);
	bool IsVisible();
	bool WaitForFrameVisibility(unsigned long time = 0);
	vector<Frame*> & GetChildren();
	void PushToFrame();
	unsigned GetID();
	vector<shared_ptr<Region>> & GetRegions(bool refresh=false);
	vector<shared_ptr<Region>> & GetFontStrings(bool refresh = false);
	friend bool operator==(const Frame & left, const Frame & right) { return left.base == right.base; }
	bool IsShown();

};

