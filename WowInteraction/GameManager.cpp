#pragma once
#include "stdafx.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include <D3dx9math.h>

#include <math.h>
using namespace DirectX;
using namespace Tools;
namespace Wow
{
	GameManager::GameManager(void)
	{
	}


	GameManager::~GameManager(void)
	{
	}
	Camera * GameManager::GetCamera()
	{
		return new Camera(Process::ReadUInt(Process::ReadRelUInt(WowOffsets::Camera::CameraPointer)+WowOffsets::Camera::CameraOffset));
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
}
