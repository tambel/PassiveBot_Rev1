#include "stdafx.h"
#include "GameObject.h"
using namespace Tools;
using namespace std;

GameObject::GameObject(unsigned base) :WowObject(base)
{
}


GameObject::~GameObject(void)
{

}
wstring &  GameObject::GetName(bool refresh)
{
	if (name.length()==0 || refresh )
	{
		name = Process::ReadString_UTF8(Process::Read<unsigned>(Process::Read<unsigned>(base + WowOffsets2::GameObject2::ObjectCache) + WowOffsets2::GameObject2::ObjectName), 0);
	}
	return name;
}
