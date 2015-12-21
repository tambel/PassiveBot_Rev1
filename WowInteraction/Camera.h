#pragma once
#include "Utils.h"
using namespace Utils::WowTypes;
namespace Wow
{
	class Camera
	{
		unsigned base;
		Position position;
		float * matrix;
		float FOV;
		float near_clip;
		float far_clip;
		float aspect;
	public:
		Camera(unsigned base);
		~Camera(void);
		unsigned GetBase();
		Position GetPosition(bool refresh=false);
		float * GetMatrix(bool refresh=false);
		float GetFOV(bool refresh=false);
		float GetNearClip(bool refresh=false);
		float GetFarClip(bool refresh=false);
		float GetAspect(bool refresh=false);
	};
}

