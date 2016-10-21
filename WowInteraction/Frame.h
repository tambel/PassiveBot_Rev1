#pragma once
#include "stdafx.h"
#include <memory>
using namespace Tools;
using namespace std;

enum RegionType {
	FONT_STRING,
	TEXTURE,
	NONE
};
class Region
{
	
	unsigned base;
	wstring text;
	RegionType type;
	string name;
public:
	Region(unsigned base);
	wstring & GetText(bool refresh=false);
	RegionType GetType();
	string & GetName(bool refresh = false);


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

};

