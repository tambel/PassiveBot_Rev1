#pragma once

#include "NetworkCommunicator.h"
#include "Packets.h"


class RemoteControl;


class NetworkCommunicatorServer: public NetworkCommunicator
{

	void InitServer();


public:
	NetworkCommunicatorServer();
	NetworkCommunicatorServer(string address, unsigned short port);
	~NetworkCommunicatorServer();
	void Start(RemoteControl * parent_rm);
	void PacketRecievedCallback(UnkPacket & packets);
};

