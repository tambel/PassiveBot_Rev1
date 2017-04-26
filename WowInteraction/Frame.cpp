#include "stdafx.h"
#include "Frame.h"
#include "Tools\Memory.h"


int Frame::const_screen_width = 65536;
int Frame::const_screen_height = 65536;
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
void Frame::RefreshSize()
{
	float fm_sw = FrameManager::GetScreenWidth();
	float fm_sh= FrameManager::GetScreenHeigth();
	bottom_prop = GetBottom()*const_screen_height / fm_sh;
	top_prop = GetTop()*const_screen_height / fm_sh;
	left_prop = GetLeft()*const_screen_width / fm_sw;
	right_prop = GetRight()*const_screen_width / fm_sw;
	width = right_prop - left_prop;
	height = top_prop - bottom_prop;
}
float Frame::GetBottomProp()
{
	return bottom_prop;
}
float Frame::GetTopProp()
{
	return top_prop;
}
float Frame::GetLeftProp()
{
	return left_prop;
}
float Frame::GetRightProp()
{
	return right_prop;
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
	RefreshSize();
	float x = left_prop + width / 2;
	float y = const_screen_height - top_prop + height / 2;
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

vector<shared_ptr<Region>> &  Frame::GetRegions(bool refresh)
{
	if (this->regions.size() > 0)
	{
		return regions;
	}
	unsigned current_region = Process::Read<unsigned>(this->base + WowOffsets2::FrameManager2::FirstRegion);
	unsigned regions_offset = Process::Read<unsigned>(this->base + WowOffsets2::FrameManager2::NextRegion);

	while (current_region)
	{
		
		this->regions.push_back(std::make_shared<Region>(current_region,this));
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

vector<shared_ptr<Region>>& Frame::GetFontStrings(bool refresh)
{
	if (font_strings.size() == 0)
	{
		for (auto & region : GetRegions())
		{
			if (region->GetType() == RegionType::FONT_STRING)
			{
				this->font_strings.push_back(region);
			}
		}
	}
	return font_strings;
}

bool Frame::IsShown()
{
	return Process::Read<unsigned>(base + 128)>>18 & 1;
}


Region::Region(unsigned base, Frame * parent)
{
	this->base = base;
	this->parent = parent;
}

inline unsigned Region::GetTextAddress()
{
	try
	{
		return Process::Read<unsigned>(this->base + WowOffsets2::FrameManager2::FontStringRegionText);
	}
	catch (MemoryReadException e)
	{
		return 0;
	}
}

wstring & Region::GetWText(bool refresh)
{
	if (this->wtext.length()==0 || refresh)
	{
		try
		{
			this->wtext = Process::ReadString_UTF8(Process::Read<unsigned>(this->base + WowOffsets2::FrameManager2::FontStringRegionText), 0);
		}
		catch (MemoryReadException e)
		{
			this->wtext = L"No Text(This text was generated by bot)";
		}
	}
	return this->wtext;

}

string & Region::GetText(bool refresh)
{
	if (this->text.length() == 0 || refresh)
	{
		try
		{
			//this->text = Process::ReadASCII(Process::Read<unsigned>(this->base + WowOffsets2::FrameManager2::FontStringRegionText), 0);
			this->text=Process::ReadASCII(GetTextAddress(),0);
		}
		catch (MemoryReadException e)
		{
			this->text = "No Text";
		}
	}
	return this->text;
}

RegionType Region::GetType()
{
	unsigned func_addr = 0;

	try
	{
		func_addr = Process::GetProcessBase()+ WowOffsets2::FrameManager2::UIObject__GetObjectType__FontString;
		if (func_addr== Process::Read<unsigned>(Process::Read<unsigned>(base) + WowOffsets2::FrameManager2::GetRegionTypeVTableOffset))
		{
			this->type = RegionType::FONT_STRING;
		}
		else
			this->type = RegionType::NONE;
	}
	catch (MemoryReadException e)
	{
		this->type = RegionType::NONE;
	}
	return this->type;
}

string & Region::GetName(bool refresh)
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
