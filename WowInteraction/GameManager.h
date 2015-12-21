#pragma once
#include "Utils.h"
#include "Camera.h"
using namespace Utils::WowTypes;
namespace Wow
{
	class GameManager
	{
		
	public:

		GameManager(void);
		~GameManager(void);
		static Camera * GetCamera();
		static bool WorldToScreen(Position pos);
		static void ClickToMove(Position pos);
		
	};
}

