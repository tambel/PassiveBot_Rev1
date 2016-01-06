#include "stdafx.h"
using namespace std;
using namespace Tools;

namespace Wow
{
	WowObject::WowObject(unsigned base)
	{
		SetBase(base);
		type=0;
		id=0;
		guid=Guid128();
	}


	WowObject::~WowObject(void)
	{
	}

	void WowObject::SetBase(unsigned base)
	{
		this->base=base;

	}
	unsigned WowObject::GetBase()
	{
		return base;
	}
	char WowObject::GetType(bool refresh)
	{
		if (!type || refresh)
		{
			type=Process::Read<unsigned>(base+WowOffsets::Object::Type);
		}
		return type;
	}

	char WowObject::GetType_Static(unsigned base)
	{
		return Process::Read<unsigned>(base+WowOffsets::Object::Type);
	}
	unsigned WowObject::GetID(bool refresh)
	{
		if (!id || refresh)
		{
			id= Process::Read<unsigned>(Process::Read<unsigned>(base+WowOffsets::Descriptors)+WowOffsets::Object::EntityID);
		}
		return id;
	}
	Guid128 * WowObject::GetGuid(bool refresh)
	{
		if (!guid || refresh)
		{
			Process::ReadRaw(Process::Read<unsigned>(base+WowOffsets::Descriptors),&guid,16);
		}
		return &guid;
	}
	Position WowObject::GetPosition(){return Position();}
}
