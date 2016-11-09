#pragma once
#include "NetworkCommuniñation\NetworkCommunicatorClient.h"
#include "NetworkCommuniñation\RemoteControl.h"
class ViewerClient:public NetworkCommunicatorClient
{
public:
	ViewerClient();
	~ViewerClient();
	Position GetPlayerPosition();
};

