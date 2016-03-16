#include "stdafx.h"
using namespace std;
using namespace Tools;
using namespace Utils;

Unit::Unit(unsigned base) :WowObject(base)
{
}


Unit::~Unit(void)
{
}
const wstring & Unit::GetName(bool refresh)
{
	if (name.length() == 0 || refresh)
	{
		name = Process::ReadString_UTF8(Process::Read<unsigned>(Process::Read<unsigned>(base + WowOffsets::Unit::UnitNameCache) + WowOffsets::Unit::UnitNameOffset), 0);
	}
	return name;
}
Position Unit::GetPosition()
{
	Position pos = Position();
	Process::ReadRaw(base + WowOffsets::Unit::UnitPosition, &pos, 12);
	pos.rotation.z = Process::Read<float>(base + WowOffsets::Unit::UnitRotation);
	return pos;
}
void Unit::DumpPosition()
{
	Position position = GetPosition();
	wcout << name << ":";
	cout<< "X- " << position.coords.x << " Y- " << position.coords.y << " Z- " << position.coords.z << endl << "Rotation " << position.rotation.z << endl<<endl;
}
unsigned Unit::IsQuestGiver()
{
	return (Process::Read<unsigned>(Process::Read<unsigned>(base + 0x124) + 0x1B8) >> 1) & 1;
	unsigned i = Process::Read<unsigned>(Process::Read<unsigned>(base + 0x124) + 0x1B8);
	return 0;
}

