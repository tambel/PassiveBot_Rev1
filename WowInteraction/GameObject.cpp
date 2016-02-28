
#include "stdafx.h"
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
	if (refresh)
	{
		name = Process::ReadString_UTF8(Process::Read<unsigned>(Process::Read<unsigned>(base + WowOffsets::GameObject::GameObjectNameCache) + WowOffsets::GameObject::GameObjectNameOffset), 0);
	}
	return name;
}
