#pragma once

class Frame
{
	unsigned base;
	string name;
	string label_text;
	Frame * parent;
	float bottom;
	float top;
	float left;
	float right;
public:
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
	Frame * GetParent();
	void MoveMoseToFrame();
	bool IsVisible();
	bool WaitForFrameVisibility(unsigned long time = 0);
};

