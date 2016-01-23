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
		float disatnce=GetPointsDistance(ppos, position);
		//float orientation = acos((position.x - ppos.x) / disatnce);
		float orientation = atan2((position.y - ppos.y),(position.x - ppos.x));
		//orientation -= Utils::PI/2;
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
		//AU3_Send(L"{NUMLOCK}");
		//AU3_Send(L"{W DOWN}");
		
		while (GetPointsDistance(ObjectManager::GetPlayer()->GetPosition().coords, point) > 0.5)
		{
			Sleep(50);
		}
		//AU3_Send(L"{NUMLOCK}");
		//AU3_Send(L"{w Up}");
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
		/*Player * player = ObjectManager::GetPlayer();
		float player_orientation = player->GetPosition().rotation.z;
		float goal_orientation = GetOrientationToTarget(point);
		float goal_deg = (goal_orientation * 180.0f) / Utils::PI;
		float player_deg = (player_orientation * 180.0f) / Utils::PI;
		float prov = goal_deg - player_deg;*/
		
		Player * player = ObjectManager::GetPlayer();
		float player_orientation= player->GetPosition().rotation.z;
		float goal_orientation = GetOrientationToTarget(point);
		float goal_deg =180- (goal_orientation * 180.0f) / Utils::PI;
		float player_deg = 180-(player_orientation * 180.0f) / Utils::PI;
		float prov = goal_deg - player_deg;
		int direction=0;
		if (prov > 0)
			prov -= 360;
		else
			prov += 360;
		if (prov > 0)
			direction = 1;
		if (prov < 0)
			direction = -1;
		RECT rect = RECT();
		//float turn=goal_orientation-
		AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
		AU3_MouseMove(rect.right / 2, rect.bottom / 2);
		AU3_MouseDown(L"Right");
		
		while (abs(player->GetPosition().rotation.z - goal_orientation)> 0.5)
		{

			AU3_MouseMove(rect.right / 2 + 100*direction, rect.bottom / 2, 1);
			AU3_MouseMove(rect.right / 2 - 100 * direction, rect.bottom / 2, 1);
			Sleep(50);

		}
	/*	while (abs(player->GetPosition().rotation.z - goal_orientation)> 0.1)
		{

			AU3_MouseMove(rect.right / 2 + 10 * direction, rect.bottom / 2, 1);
			AU3_MouseMove(rect.right / 2 - 10 * direction, rect.bottom / 2, 1);
			Sleep(50);

		}
		while (abs(player->GetPosition().rotation.z - goal_orientation) > 0.01)
		{
			AU3_MouseMove(rect.right / 2 + direction, rect.bottom / 2, 1);
			AU3_MouseMove(rect.right / 2 - direction, rect.bottom / 2, 1);
			Sleep(50);
		}*/
		AU3_MouseUp(L"Right");
		

	}
	void GameManager::RotatePlayerTest(Vector3 & point)
	{
		Player * player = ObjectManager::GetPlayer();
		float da = 0.00392698825257;
		//float pr = Utils::PI - player->GetPosition().rotation.z;
		//float tr= Utils::PI - GetOrientationToTarget(point);
		float pr = player->GetPosition().rotation.z;
		float tr =GetOrientationToTarget(point);
		//float angle = tr - pr;
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
				//angle = pr - tr;
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
				//angle = tr - pr;
				angle = -(tr + Utils::PI * 2-pr);
			}

		}

		int npixel = roundf(angle/da);
		RECT rect = RECT();
		AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
		AU3_MouseMove(rect.right / 2 + npixel, rect.bottom / 2, 1);
		cout << "pr "<<pr<<" tr "<<tr<<" angle "<<angle<<" pix "<< npixel << endl;
		if (abs(GetOrientationToTarget(point) - player->GetPosition().rotation.z)>0.1)
		{
			Sleep(50);
			RotatePlayerTest(point);
		}




		//float player_orientation = player->GetPosition().rotation.z;
		//float goal_orientation = GetOrientationToTarget(point);
		//float goal_deg = 180 - (goal_orientation * 180.0f) / Utils::PI;
		//float player_deg = 180 - (player_orientation * 180.0f) / Utils::PI;
		//float prov = goal_deg - player_deg;
		//int direction = 0;
		//if (prov > 0)
		//	prov -= 360;
		//else
		//	prov += 360;
		//if (prov > 0)
		//	direction = 1;
		//if (prov < 0)
		//	direction = -1;
		//RECT rect = RECT();
		////float turn=goal_orientation-
		//AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
		////AU3_MouseMove(rect.right / 2, rect.bottom / 2);
		////AU3_MouseDown(L"Right");
		//ofstream mouse;
		//mouse.open("mouse.txt");
		//while (abs(player->GetPosition().rotation.z - goal_orientation)> 0.1)
		//{

		//	AU3_MouseMove(rect.right / 2 + 10 * direction, rect.bottom / 2, 1);
		//	POINT p;
		//	AU3_MouseGetPos(&p);
		//	mouse << p.x << " " << p.y << endl;
		//	//AU3_MouseMove(rect.right / 2 - 1 * direction, rect.bottom / 2, 1);
		//	Sleep(1);

		//}
		/*while (abs(player->GetPosition().rotation.z - goal_orientation)> 0.1)
		{

			AU3_MouseMove(rect.right / 2 + 10 * direction, rect.bottom / 2, 0);
			AU3_MouseMove(rect.right / 2 - 10 * direction, rect.bottom / 2, 0);
			Sleep(50);

		}
		while (abs(player->GetPosition().rotation.z - goal_orientation) > 0.01)
		{
			AU3_MouseMove(rect.right / 2 + direction, rect.bottom / 2, 0);
			AU3_MouseMove(rect.right / 2 - direction, rect.bottom / 2, 0);
			Sleep(50);
		}*/
		//AU3_MouseUp(L"Right");
	}
	void GameManager::GoToPoint(Vector3 & point)
	{
		RotatePlayer(point);
		GoStraighToPoint(point);
	}
	void GameManager::GoToPointTest(Vector3 & point)
	{
		RotatePlayer(point);
		//AU3_Send(L"{UP down}");

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
