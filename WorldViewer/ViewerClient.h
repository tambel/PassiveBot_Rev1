#pragma once
#include "NetworkCommuni�ation\NetworkCommunicatorClient.h"
#include "NetworkCommuni�ation\RemoteControl.h"
class ViewerClient:public NetworkCommunicatorClient
{
public:
	ViewerClient();
	~ViewerClient();
	Position GetPlayerPosition();
};

