#include <d3dx11.h>
		
#include <math.h>
#include "stdafx.h"
#include <d3d11.h>
#pragma once
#include <DirectXMath.h>
#include <D3dx9math.h>

#define _USE_MATH_DEFINES
using namespace std;
using namespace DirectX;
using namespace Tools;
namespace Wow
{
	float GameManager::GetOrientationToTarget(Vector3 & position)
	{
		Vector3 ppos = ObjectManager::GetPlayer()->GetPosition().coords;
		//float hyp = sqrt((position.x - ppos.x)*(position.x - ppos.x) + (position.y - ppos.y)*(position.y - ppos.y));
		float disatnce=GetDistanceToPoint(ppos, position);
		//float orientation = acos((position.x - ppos.x) / disatnce);
		float orientation = atan2((position.y - ppos.y),(position.x - ppos.x));
		//orientation -= Utils::PI/2;
		while (orientation <0) orientation += Utils::PI*2;
		while (orientation > Utils::PI * 2) orientation -= Utils::PI * 2;
		return orientation;
	}
	float GameManager::GoStraighToPoint(Vector3 & point)
	{
		
		//AU3_Send(L"UP down");
		RECT rect = RECT();
		AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
		AU3_MouseMove(rect.right / 2, rect.bottom / 2);
		AU3_MouseDown(L"Right");
		AU3_MouseDown(L"Left");
		while (GetDistanceToPoint(ObjectManager::GetPlayer()->GetPosition().coords, point) > 0.5)
		{
			Sleep(10);
		}
		AU3_MouseUp(L"Right");
		AU3_MouseUp(L"Left");
		return 0;
	}
	float GameManager::GetDistanceToPoint(Vector3 & start, Vector3 & end)
	{
		
		float hyp = sqrt((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y));
		return hyp;

	}
	void GameManager::RotatePlayer(Vector3 & point)
	{
		Player * player = ObjectManager::GetPlayer();
		float goal_orientation = GetOrientationToTarget(point);
		RECT rect = RECT();
		AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
		AU3_MouseMove(rect.right / 2, rect.bottom / 2);
		AU3_MouseDown(L"Right");
		int i = 0;

		while (abs(player->GetPosition().rotation.z - goal_orientation)> 0.1)
		{

			AU3_MouseMove(rect.right / 2 + 6, rect.bottom / 2, 0);
			AU3_MouseMove(rect.right / 2 - 6, rect.bottom / 2, 0);
			//AU3_MouseClickDrag(L"Right", rect.right / 2, rect.bottom / 2, rect.right / 2+ 6, rect.bottom / 2,0);
			Sleep(5);
			//player->DumpPosition();

		}
		while (abs(player->GetPosition().rotation.z - goal_orientation) > 0.01)
		{
			AU3_MouseMove(rect.right / 2 + 1, rect.bottom / 2, 0);
			AU3_MouseMove(rect.right / 2 - 1, rect.bottom / 2, 0);
			//AU3_MouseClickDrag(L"Right", rect.right / 2, rect.bottom / 2, rect.right / 2+ 6, rect.bottom / 2,0);

			//player->DumpPosition();
			//cout << "1111 " << player->GetPosition().rotation.z << endl << goal_orientation << endl;
			Sleep(5);
		}

		Sleep(100);
		AU3_MouseUp(L"Right");

	}
	void GameManager::GoToPoint(Vector3 & point)
	{
		RotatePlayer(point);
		GoStraighToPoint(point);
	}
	GameManager::GameManager(void)
	{
	}


	GameManager::~GameManager(void)
	{
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
