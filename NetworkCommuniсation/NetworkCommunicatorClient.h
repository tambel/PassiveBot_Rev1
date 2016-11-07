#pragma once
#include "NetworkCommunicator.h" 


using boost::asio::ip::tcp;
class NetworkCommunicatorClient:public NetworkCommunicator
{
protected:
	boost::asio::io_service * io_service;
	tcp::resolver  *resolver;
	//tcp::resolver::query query(ep);
	tcp::resolver::iterator endpoint_iterator;
	tcp::socket  * socket;
public:
	NetworkCommunicatorClient(string address, unsigned short port);
	~NetworkCommunicatorClient();
	void Connect();
};

