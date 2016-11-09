#include "stdafx.h"
#include "ViewerClient.h"


ViewerClient::ViewerClient():
	NetworkCommunicatorClient("127.0.0.1", 8002)
{
}


ViewerClient::~ViewerClient()
{
}

Position ViewerClient::GetPlayerPosition()
{
	RequestPacket rp = RequestPacket(Request::PlayerPosition);
	PlayerPositionReply *  reply;
	SendPacket(*socket, &rp, rp.header.size);
	char * buff = RecievePacket(*socket);
	Position res;
	reply = reinterpret_cast<PlayerPositionReply*>(buff);
	res.coords = reply->position;
	res.rotation = reply->rotation;
	delete [] buff;
	return res;
}
