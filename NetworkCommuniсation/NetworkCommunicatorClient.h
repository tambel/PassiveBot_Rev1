#pragma once
#include "NetworkCommunicator.h" 


using boost::asio::ip::tcp;
class NetworkCommunicatorClient:public NetworkCommunicator
{
public:
	NetworkCommunicatorClient(string address, unsigned short port);
	~NetworkCommunicatorClient();
	void Connect();
};

