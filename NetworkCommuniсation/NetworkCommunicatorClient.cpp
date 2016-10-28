#include "stdafx.h"
#include "NetworkCommunicatorClient.h"



NetworkCommunicatorClient::NetworkCommunicatorClient(string address, unsigned short port):
	NetworkCommunicator(address,port)
{

}


NetworkCommunicatorClient::~NetworkCommunicatorClient()
{
}

void NetworkCommunicatorClient::Connect()
{
	NetworkCommunicator::Start();
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	//tcp::resolver::query query(ep);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(endpoint);
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
}

