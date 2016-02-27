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
		static void RotatePlayer(WowObject * object);
		static float GetOrientationToTarget(Vector3 & position);
		static float GoStraighToPoint(Vector3 & point);
		inline static float GetPointsDistance(WowObject & object1, WowObject & object2);
		static float GetPointsDistance(Vector3 & start, Vector3 & end);
		static float GetPlayerDistanceToPoint(Vector3 & end);
		inline static float GetPlayerDistanceToPoint(WowObject & object);
		static void RotatePlayer(Vector3 & point);
		static void GoToPoint(Vector3 & point);
		

	};
}

