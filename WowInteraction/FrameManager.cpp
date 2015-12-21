#include "stdafx.h"
using namespace Tools;
namespace Wow
{
	vector<Frame*> * FrameManager::frames=new vector<Frame*>();
	float FrameManager::screen_heigth=0;
	float FrameManager::screen_width=0;
	FrameManager::FrameManager(void)
	{

	}


	FrameManager::~FrameManager(void)
	{
	}
	vector <Frame*> * FrameManager::GetFrames()
	{
		return frames;
	}
	void FrameManager::EnumAllFrames()
	{
		ClearFrames();
		unsigned base_frame=Process::ReadRelUInt(WowOffsets::FrameManager::FrameBase);
		unsigned current=Process::ReadUInt(base_frame+WowOffsets::FrameManager::FirstFrame);
		while (current)
		{
			frames->push_back(new Frame(current));
			current=Process::ReadUInt(current+Process::ReadUInt(base_frame+WowOffsets::FrameManager::NextFrame)+4);
		
		}
	}
	void FrameManager::DumpAllFramesNames()
	{
		cout<<"Frame Names "<<frames->size()<<endl;
		for (auto frame:*frames)
		{
			cout<<frame->GetName()<<endl;
		}
		cout<<frames->size();
	}
	void FrameManager::ClearFrames()
	{
		for (auto frame:*frames)
		{
			delete frame;
		} 
		frames->clear();
	}
	void FrameManager::FindParents()
	{
		unsigned parent_ptr;
		for (auto frame:*frames)
		{
			for (auto frame2:*frames)
			{
				parent_ptr=Process::ReadUInt(frame->GetBase()+WowOffsets::FrameManager::ParentFrame);
				if (parent_ptr==frame2->GetBase())
				{
					frame->SetParent(frame2);
					break;
				}
			}
		}
	}
	float FrameManager::GetScreenWidth(bool refresh)
	{
		if (!screen_width || refresh)
		{
			screen_width=Process::ReadRelFloat(WowOffsets::FrameManager::ScrWidth);
		}
		return screen_width;
	}
	float FrameManager::GetScreenHeigth(bool refresh)
	{
		if (!screen_heigth || refresh)
		{
			screen_heigth=Process::ReadRelFloat(WowOffsets::FrameManager::ScrHeight);
		}
		return screen_heigth;
	}
	Frame * FrameManager::FindFrameByName(const char * name)
	{
		EnumAllFrames();
		for (auto frame:*frames)
		{
			if (strcmp(frame->GetName(),name)==0)
			{
				
				return frame;
			}
		}		
		return 0;
	}
	Frame * FrameManager::FindFrameByAddress(unsigned address)
	{
		for (auto frame:*frames)
		{
			if (address==frame->GetBase())
			{
				return frame;
			}
		}
		return 0;
	}
}
