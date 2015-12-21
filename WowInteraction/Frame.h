#pragma once
namespace Wow
{
	class Frame
	{
		unsigned base;
		char * name;
		char * label_text;
		Frame * parent;
		float bottom;
		float top;
		float left;
		float right;
	public:
		Frame(unsigned base);
		~Frame(void);
		char * GetName(bool refresh=false);
		char * GetLabelText(bool refresh=false);
		void SetParent(Frame * parent);
		unsigned GetBase();
		float GetBottom(bool refresh=false);
		float GetTop(bool refresh=false);
		float GetLeft(bool refresh=false);
		float GetRight(bool refresh=false);
		Frame * GetParent();
		void MoveMoseToFrame();
		bool IsVisible();
		bool WaitForFrameVisibility(unsigned long time=0);
	};
}

