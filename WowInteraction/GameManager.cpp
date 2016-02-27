#include <d3dx11.h>
		
#include <math.h>
#include "stdafx.h"
#include <d3d11.h>
#pragma once
#include <DirectXMath.h>
#include <D3dx9math.h>
#include "boost\geometry\geometry.hpp"
#include "boost\numeric\ublas\matrix.hpp"
#include "boost\numeric\ublas\vector.hpp"

#define _USE_MATH_DEFINES


using namespace std;
using namespace DirectX;
using namespace Tools;
namespace Wow
{
	float GameManager::GetOrientationToTarget(Vector3 & position)
	{
		Vector3 ppos = ObjectManager::GetPlayer()->GetPosition().coords;
		float disatnce=GetPointsDistance(ppos, position);
		float orientation = atan2((position.y - ppos.y),(position.x - ppos.x));
		while (orientation <0) orientation += Utils::PI*2;
		while (orientation > Utils::PI * 2) orientation -= Utils::PI * 2;
		return orientation;
	}
	float GameManager::GoStraighToPoint(Vector3 & point)
	{
		RECT rect = RECT();
		AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
		AU3_MouseMove(rect.right / 2, rect.bottom / 2);
		AU3_MouseDown(L"Left");
		AU3_MouseDown(L"Right");
		while (GetPointsDistance(ObjectManager::GetPlayer()->GetPosition().coords, point) > 0.5)
		{
			Sleep(50);
		}
		AU3_MouseUp(L"Left");
		AU3_MouseUp(L"Right");
		return 0;
	}
	float GameManager::GetPointsDistance(WowObject & object1, WowObject & object2)
	{
		return GetPointsDistance(object1, object2);
	}
	float GameManager::GetPointsDistance(Vector3 & start, Vector3 & end)
	{
		
		float hyp = sqrt((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y));
		return hyp;

	}
	float GameManager::GetPlayerDistanceToPoint(Vector3 & end)
	{
		
		return GetPointsDistance(ObjectManager::GetPlayer()->GetPosition().coords, end);
	}
	float GameManager::GetPlayerDistanceToPoint(WowObject & object)
	{
		return GetPointsDistance(*ObjectManager::GetPlayer(), object);
	}
	void GameManager::RotatePlayer(Vector3 & point)
	{
		Player * player = ObjectManager::GetPlayer();
		float da = 0.00392698825257f;
		float pr = player->GetPosition().rotation.z;
		float tr =GetOrientationToTarget(point);
		float angle;
		if (tr > pr)
		{
			if (tr -pr< Utils::PI)
			{
				angle = -(tr - pr);
			}
			else
			{
				angle = (pr + Utils::PI * 2 - tr);
			}
		}
		if (pr > tr)
		{
			if (pr - tr< Utils::PI)
			{
				angle = pr - tr;
			}
			else
			{
				angle = -(tr + Utils::PI * 2-pr);
			}

		}
		int npixel = static_cast<int>(roundf(angle/da));
		RECT rect = RECT();
		AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
		AU3_MouseMove(rect.right / 2 + npixel, rect.bottom / 2, 1);
		if (abs(GetOrientationToTarget(point) - player->GetPosition().rotation.z)>0.1)
		{
			Sleep(50);
			RotatePlayer(point);
		}
	}
	void GameManager::GoToPoint(Vector3 & point)
	{
		RotatePlayer(point);
		GoStraighToPoint(point);
	}
	Camera * GameManager::GetCamera()
	{
		return new Camera(Process::Read<unsigned>(Process::ReadRel<unsigned>(WowOffsets::Camera::CameraPointer)+WowOffsets::Camera::CameraOffset));
	}
	bool GameManager::WorldToScreen(Position pos)
	{
		Camera * cam=GameManager::GetCamera();
		Rect size;
		unsigned y;
		unsigned x;
		size.width=GetSystemMetrics(SM_CXSCREEN);
		size.heigth=GetSystemMetrics(SM_CYSCREEN);
		XMVECTOR vEye=XMVectorSet( cam->GetPosition().coords.x, cam->GetPosition().coords.y, cam->GetPosition(true).coords.z, 0.0f );
		XMVECTOR vLookat=vEye+XMVectorSet(cam->GetMatrix()[0],cam->GetMatrix()[1],cam->GetMatrix(true)[2],0.0f);
		XMVECTOR vUpVec =XMVectorSet(0.0f,0.0f,1.0f,0.0f);
		XMVECTOR objPos=XMVectorSet(pos.coords.x,pos.coords.y,pos.coords.z,0.0f);	
		XMMATRIX proj= XMMatrixPerspectiveFovRH(cam->GetFOV(true)* 0.6f,size.width/size.heigth,cam->GetNearClip(true),cam->GetFarClip(true));
		XMMATRIX view= XMMatrixLookAtRH(vEye,vLookat,vUpVec);
		XMMATRIX world=XMMatrixIdentity();
		XMVECTOR result=XMVector3Project(objPos,0,0,size.width,size.heigth,cam->GetNearClip(),cam->GetFarClip(),proj,view,world);
		y=result.m128_f32[1]*((float)65536/GetSystemMetrics(SM_CYSCREEN));
		proj= XMMatrixPerspectiveFovRH(cam->GetFOV()* 0.94f,size.width/size.heigth,cam->GetNearClip(),cam->GetFarClip());
		result=XMVector3Project(objPos,0,0,size.width,size.heigth,cam->GetNearClip(),cam->GetFarClip(),proj,view,world);
		x=result.m128_f32[0]*((float)65536/GetSystemMetrics(SM_CXSCREEN));
		Process::MoveMouse(x,y);
		delete cam;
		return 0;
	}
	void GameManager::ClickToMove(Position pos)
	{
		WorldToScreen(pos);
		Process::MouseClick(MouseButton::RIGHT);

	}
	void GameManager::RotatePlayer(WowObject * object)
	{
		RotatePlayer(object->GetPosition().coords);
	}
}
