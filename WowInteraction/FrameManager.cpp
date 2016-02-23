#include "stdafx.h"
using namespace Tools;
namespace Wow
{
	vector<Frame*> FrameManager::frames=vector<Frame*>();
	float FrameManager::screen_heigth=0;
	float FrameManager::screen_width=0;
	FrameManager::FrameManager(void)
	{

	}


	FrameManager::~FrameManager(void)
	{
	}
	vector <Frame*> & FrameManager::GetFrames()
	{
		return frames;
	}
	void FrameManager::EnumAllFrames()
	{
		unsigned base_frame;
		unsigned current;
		ClearFrames();
		try
		{
			 base_frame = Process::ReadRel<unsigned>(WowOffsets::FrameManager::FrameBase);
			 current = Process::Read<unsigned>(base_frame + WowOffsets::FrameManager::FirstFrame);
		}
		catch (MemoryReadException e)
		{
			throw FrameEnumerationException();
		}
		while (current)
		{
			try
			{
				frames.push_back(new Frame(current));
				current = Process::Read<unsigned>(current + Process::Read<unsigned>(base_frame + WowOffsets::FrameManager::NextFrame) + 4);
			}
			catch (MemoryReadException e)
			{
				break;
			}
		}
	}
	void FrameManager::DumpAllFramesNames()
	{
		cout<<"Frame Names "<<frames.size()<<endl;
		for (auto frame:frames)
		{
			cout<<frame->GetName()<<endl;
		}
		cout<<frames.size();
	}
	void FrameManager::ClearFrames()
	{
		for (auto frame:frames)
		{
			delete frame;
		} 
		frames.clear();
	}
	void FrameManager::FindParents()
	{
		unsigned parent_ptr;
		for (auto frame:frames)
		{
			for (auto frame2:frames)
			{
				parent_ptr=Process::Read<unsigned>(frame->GetBase()+WowOffsets::FrameManager::ParentFrame);
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
			screen_width=Process::ReadRel<float>(WowOffsets::FrameManager::ScrWidth);
		}
		return screen_width;
	}
	float FrameManager::GetScreenHeigth(bool refresh)
	{
		if (!screen_heigth || refresh)
		{
			screen_heigth=Process::ReadRel<float>(WowOffsets::FrameManager::ScrHeight);
		}
		return screen_heigth;
	}
	Frame * FrameManager::FindFrameByName(const string &  name)
	{
		EnumAllFrames();
		for (auto frame:frames)
		{
			if (frame->GetName()==name)
			{
				
				return frame;
			}
		}		
		return 0;
	}
	Frame * FrameManager::FindFrameByAddress(unsigned address)
	{
		for (auto frame:frames)
		{
			if (address==frame->GetBase())
			{
				return frame;
			}
		}
		return 0;
	}
}
