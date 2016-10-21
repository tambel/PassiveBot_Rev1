#include "stdafx.h"



Frame::Frame()
{
}

Frame::Frame(unsigned base)
{
	this->base = base;
	parent = 0;
	bottom = -0xFFFF;
	top = -0xFFFF;
	left = -0xFFFF;
	right = -0xFFFF;
	id = 0;
}


Frame::~Frame(void)
{
}
string & Frame::GetName(bool refresh)
{
	if (name.length() == 0 || refresh)
	{
		try
		{
			name = Process::ReadASCII(Process::Read<unsigned>(base + WowOffsets2::FrameManager2::FrameName), 0);
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
	if (refresh)
	{
		label_text = Process::ReadASCII(Process::Read<unsigned>(base + WowOffsets::FrameManager::LabelText), 0);
	}
	return label_text;
}
unsigned Frame::GetBase()
{
	return base;
}
void  Frame::SetParent(Frame * parent)
{
	this->parent = parent;
}
float Frame::GetBottom(bool refresh)
{
	if (bottom == -0xFFFF || refresh)
	{
		bottom = Process::Read<float>(base + WowOffsets2::FrameManager2::FrameBottom);
	}
	return bottom;
}
float Frame::GetTop(bool refresh)
{
	if (top == -0xFFFF || refresh)
	{
		top = Process::Read<float>(base + WowOffsets2::FrameManager2::FrameTop);
	}
	return top;
}
float Frame::GetLeft(bool refresh)
{
	if (left == -0xFFFF || refresh)
	{
		left = Process::Read<float>(base + WowOffsets2::FrameManager2::FrameLeft);
	}
	return left;
}
float Frame::GetRight(bool refresh)
{
	if (right == -0xFFFF || refresh)
	{
		right = Process::Read<float>(base + WowOffsets2::FrameManager2::FrameRight);
	}
	return right;
}
inline Frame * Frame::GetParent()
{
	if (!parent)
	{
		try
		{
			unsigned parent_ptr = Process::Read<unsigned>(base + WowOffsets2::FrameManager2::FrameParent);
			parent = FrameManager::FindFrameByAddress(parent_ptr);
		}
		catch (MemoryReadException e)
		{
			return 0;
		}
		
	}
	return parent;
}
void Frame::MoveMouseToFrame()
{
	int sw = 65536;
	int sh = 65536;
	float ssh = FrameManager::GetScreenHeigth();
	float ssw = FrameManager::GetScreenWidth();
	float b = GetBottom()*sh / FrameManager::GetScreenHeigth();
	float t = GetTop()*sh / FrameManager::GetScreenHeigth();
	float l = GetLeft()*sw / FrameManager::GetScreenWidth();
	float r = GetRight()*sw / FrameManager::GetScreenWidth();
	float w = r - l;
	float h = t - b;
	float x = l + w / 2;
	float y = sh - t + h / 2;
	Process::MoveMouse((unsigned)x, (unsigned)y);
}
bool Frame::MoveMouseToFrameAndClick(unsigned delay, MouseButton button)
{
	this->MoveMouseToFrame();
	Sleep(delay);
	Process::MouseClick(button);

	return true;
}
bool Frame::IsVisible()
{
	char result = Process::Read<char>(base + WowOffsets::FrameManager::Visible);
	if (result == 1)
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
	unsigned attempts = 0;
	while ((!IsVisible() && attempts < 1000))
	{
		if (time != 0 && attempts * 10 > time)
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

vector<Frame*> & Frame::GetChildren()
{
	if (children.size() == 0)
	{
		if (this->base != 0)
			for (auto frame : FrameManager::GetFrames())
			{
				if (frame->GetParent())
					if (frame->GetParent()->GetBase() == this->base)
					{
						children.push_back(frame);
					}
			}
	}
	return children;
}

void Frame::PushToFrame()
{
	MoveMouseToFrame();
	Sleep(100);
	AU3_MouseClick(L"Left");
}

unsigned Frame::GetID()
{
	try
	{
		this->id= Process::Read<unsigned>(this->base + WowOffsets2::FrameManager2::FrameId);
		return this->id;
	}
	catch (MemoryReadException e)
	{
		return 0;
	}
}

vector<Region> &  Frame::GetRegions()
{
	this->regions.clear();
	unsigned current_region = Process::Read<unsigned>(this->base + 0x130);
	unsigned regions_offset = Process::Read<unsigned>(this->base + 0x128);

	while (current_region)
	{
		
		this->regions.push_back(Region(current_region));
		try
		{
			current_region = Process::Read<unsigned>(current_region + regions_offset + 4);
		}
		catch (MemoryReadException e)
		{
			current_region = 0;
		}
		

	}
	return this->regions;
}

Region::Region(unsigned base)
{
	this->base = base;
}

string & Region::GetText(bool refresh)
{
	if (this->text.length()==0 || refresh)
	{
		try
		{
			this->text = Process::ReadASCII(Process::Read<unsigned>(this->base + 0xE4), 0);
		}
		catch (MemoryReadException e)
		{
			this->text = "No Text";
		}
	}
	return this->text;

}
