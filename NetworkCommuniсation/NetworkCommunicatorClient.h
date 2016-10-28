#pragma once
#include "stdafx.h"
#include "BotInteractor\stdafx.h"
#include <boost/array.hpp>
using boost::asio::ip::tcp;
class NetworkCommunicatorClient
{
	boost::asio::io_service * io_service;
	tcp::socket * socket;
	void InitClient();
public:
	Vector3 player_position;
	NetworkCommunicatorClient();
	~NetworkCommunicatorClient();

	void WaitForMessage();
};

