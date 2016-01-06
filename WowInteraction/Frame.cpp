#include "stdafx.h"

using namespace Tools;
namespace Wow
{
	Frame::Frame(unsigned base)
	{
		this->base=base;
		parent=0;
		bottom=-0xFFFF;
		top=-0xFFFF;
		left=-0xFFFF;
		right=-0xFFFF;
	}


	Frame::~Frame(void)
	{
	}
	string & Frame::GetName(bool refresh)
	{
		if (name.length()==0|| refresh)
		{
			try
			{
				name = Process::ReadASCII(Process::Read<unsigned>(base + WowOffsets::FrameManager::Name), 0);
			}
			catch (MemoryReadException e)
			{
				name = "";
			}
		}
		return name;
	}
	string & Frame::GetLabelText(bool refresh)
	{
		if ( refresh)
		{
			label_text=Process::ReadASCII(Process::Read<unsigned>(base+WowOffsets::FrameManager::LabelText),0);
		}
		return label_text;
	}
	unsigned Frame::GetBase()
	{
		return base;
	}
	void  Frame::SetParent(Frame * parent)
	{
		this->parent=parent;
	}
	float Frame::GetBottom(bool refresh)
	{
		if (bottom==-0xFFFF || refresh)
		{
			bottom=Process::Read<float>(base+WowOffsets::FrameManager::FrameBottom);
		}
		return bottom;
	}
	float Frame::GetTop(bool refresh)
	{
		if (top==-0xFFFF || refresh)
		{
			top=Process::Read<float>(base+WowOffsets::FrameManager::FrameTop);
		}
		return top;
	}
	float Frame::GetLeft(bool refresh)
	{
		if (left==-0xFFFF || refresh)
		{
			left=Process::Read<float>(base+WowOffsets::FrameManager::FrameLeft);
		}
		return left;
	}
	float Frame::GetRight(bool refresh)
	{
		if (right==-0xFFFF || refresh)
		{
			right=Process::Read<float>(base+WowOffsets::FrameManager::FrameRight);
		}
		return right;
	}
	Frame * Frame::GetParent()
	{
		if (!parent)
		{
			unsigned parent_ptr=Process::Read<unsigned>(base+WowOffsets::FrameManager::ParentFrame);
			parent= FrameManager::FindFrameByAddress(parent_ptr);
		}
		return parent;
	}
	void Frame::MoveMoseToFrame()
	{
		int sw=65536;
		int sh= 65536;
		float b=GetBottom()*sh/FrameManager::GetScreenHeigth();
		float t=GetTop()*sh/FrameManager::GetScreenHeigth();
		float l=GetLeft()*sw/FrameManager::GetScreenWidth();
		float r=GetRight()*sw/FrameManager::GetScreenWidth();
		float w=r-l;
		float h=t-b;
		float x=l+w/2;
		float y=sh-t+h/2;
		Process::MoveMouse((unsigned)x,(unsigned)y);
	}
	bool Frame::IsVisible()
	{
		char result=Process::Read<char>(base+WowOffsets::FrameManager::Visible);
		if (result==1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool Frame::WaitForFrameVisibility(unsigned long time)
	{
		unsigned attempts=0;
		while ((!IsVisible() && attempts<1000))
		{
			if (time!=0 && attempts*10>time)
			{
				break;
			}
			Sleep(10);
			attempts++;
		}
		if (IsVisible())
		{
			return true;
		}
		return false;
	}

}