#pragma pack
#include "stdafx.h"
#include "RemoteControl.h"


RemoteControl::RemoteControl()
{
	server = NetworkCommunicatorServer("127.0.0.1", 8001);
	recieved_packet = nullptr;
	packet_to_send = nullptr;
	packet_to_send_size = 0;

}


RemoteControl::~RemoteControl()
{
}

void RemoteControl::ProcessRequest()
{
	on_receive_mutex.lock();
	if (recieved_packet)
	{
		cout << "PROCESS_REQUEST" << endl;
		packet_to_send = ProcessRequest(recieved_packet);
		
		delete [] recieved_packet;
		recieved_packet = nullptr;
	}
	on_receive_mutex.unlock();
	
	
}

char ** RemoteControl::ProcessPacket(char * packet,unsigned & size)
{
	on_receive_mutex.lock();
	recieved_packet = packet;
	on_receive_mutex.unlock();
	bool release = false;
	while (!release)
	{
		on_receive_mutex.lock();
		if (packet_to_send)
		{
			release = true;
		}
		on_receive_mutex.unlock();
		cout << "WAINTING!!!!!!!" << endl;
		Sleep(1);
	}
	cout << "continue!!!" << endl;
	size = packet_to_send_size;
	return &packet_to_send;
}

char * RemoteControl::ProcessRequest(char * packet)
{
	//unsigned type = packet->type;
	unsigned type = *reinterpret_cast<unsigned*>(packet + 8);
	char * reply = nullptr;
	switch (type)
	{
	case Requests::PlayerPosition:
	{
		PlayerPositionReply * rp = new PlayerPositionReply;
		rp->header.size = sizeof(*rp);
		Position pos = ObjectManager::GetPlayer()->GetPosition();
		rp->position = pos.coords;
		rp->rotation = pos.rotation;
		reply = reinterpret_cast<char*>(rp);
		packet_to_send_size = rp->header.size;
		break;
	}
	case Requests::TargetObjectInfo:
	{
		TargetObjectInfoReply * rp = new TargetObjectInfoReply;
		rp->header.size = sizeof(*rp);
		WowObject * obj = ObjectManager::GetTargetObject();
		rp->type = obj->GetType();
		rp->guid = *obj->GetGuid();
		wstring  name = obj->GetName();
		rp->name.length = name.size() * 2;
		memcpy(rp->name.str, name.c_str(), (name.size()+1)*2);
		rp->position = obj->GetPosition();
		reply = reinterpret_cast<char*>(rp);
		packet_to_send_size = rp->header.size;
		break;
	}
	}
	return reply;
}
void RemoteControl::Start()
{
	server_thread = thread(&NetworkCommunicatorServer::Start, server,this);
	server_thread.detach();
	//server.Start(this);
}


