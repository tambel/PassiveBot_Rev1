#pragma once
#include <mutex>
#include "NetworkCommunicatorServer.h"
#include <thread>
#include "WowInteraction\WowInteraction.h"
//using up_cast= dynamic_cast<UnkPacket*>
#define up_cast reinterpret_cast<UnkPacket*>



enum Request
{
	PlayerPosition = 1,
	TargetInfo = 2,
	Logout = 3,
	TargetQuestGiverQuestList=4,
	SelectQuestFromList=5,
	TargetEntityID=6

};





class RemoteControl
{
	NetworkCommunicatorServer server;
	mutex on_receive_mutex;
	char  * recieved_packet;
	char * packet_to_send;
	unsigned packet_to_send_size;
	std::condition_variable cond_var;
	thread server_thread;

public:
	RemoteControl();
	RemoteControl(string address, unsigned short port);
	~RemoteControl();
	void ProcessRequest();
	char ** ProcessPacket(char * packet, unsigned & size);
	char * ProcessRequest(char * packet);
	void Start();


};

class ViewerCommunication : public RemoteControl
{
public:
	ViewerCommunication() :RemoteControl("127.0.0.1", 8002) { }
};

