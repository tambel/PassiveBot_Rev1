#include "Utils.h"
#pragma once
using namespace Utils::WowTypes;
namespace Wow
{
	struct Guid128
	{
		unsigned long long low;
		unsigned long long high;
		Guid128()
		{
			low=0;
			high=0;
		}
		friend bool operator==(const Guid128& left, const Guid128& right)
		{
			if (left.high==right.high && left.low==right.low)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		friend bool operator!=(const Guid128& left, const Guid128& right)
		{
			if (left.high!=right.high || left.low!=right.low)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		friend bool operator!(const Guid128& guid)
		{
			if (guid.high==0 && guid.low==0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};
enum ObjectType
{
	ITEM = 1,
	CONTAINER = 2,
	UNIT = 3,
	PLAYER = 4,
	GAMEOBJECT = 5,
	DYNAMICOBJECT = 6,
	CORPSE = 7,
	AiGroup = 8,
	AreaTrigger = 9
};
class WowObject
{

protected:
	unsigned base;
	wchar_t * name;
	Guid128 guid;
	char type;
	unsigned id;


public:
	WowObject(unsigned base);
	virtual ~WowObject(void);
	void SetBase(unsigned base);
	unsigned GetBase();
	char GetType(bool refresh=false);
	unsigned GetID(bool refresh=false);
	Guid128 * GetGuid(bool refresh=false);
	virtual wchar_t *GetName(bool refresh=false)=0;
	static char GetType_Static(unsigned base);
	virtual Position GetPosition();
	//virtual void DumpPosition(bool refresh=false)=0;

};
}
