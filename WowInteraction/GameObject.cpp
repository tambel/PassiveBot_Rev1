
#include "stdafx.h"
using namespace Tools;
namespace Wow
{

	GameObject::GameObject(unsigned base):WowObject(base)
	{
	}


	GameObject::~GameObject(void)
	{

	}
	wchar_t * GameObject::GetName(bool refresh)
	{
		if (!name || refresh)
		{
			delete [] name;
			name= Process::ReadString_UTF8(Process::ReadUInt(Process::ReadUInt(base+WowOffsets::GameObject::GameObjectNameCache)+WowOffsets::GameObject::GameObjectNameOffset),0);
		}
		return name;
	}
}