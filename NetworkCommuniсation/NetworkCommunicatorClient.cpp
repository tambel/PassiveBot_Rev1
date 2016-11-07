#include "stdafx.h"
#include "NetworkCommunicatorClient.h"



NetworkCommunicatorClient::NetworkCommunicatorClient(string address, unsigned short port):
	NetworkCommunicator(address,port)
{

}


NetworkCommunicatorClient::~NetworkCommunicatorClient()
{
	delete io_service;
	delete resolver;
	delete socket;
}

void NetworkCommunicatorClient::Connect()
{
	try
	{
		//boost::asio::io_service io_service;
		//tcp::resolver resolver(io_service);
		////tcp::resolver::query query(ep);
		//tcp::resolver::iterator endpoint_iterator = resolver.resolve(endpoint);
		//tcp::socket socket(io_service);
		//boost::asio::connect(socket, endpoint_iterator);
		io_service= new boost::asio::io_service;
		resolver= new tcp::resolver(*io_service);
		//tcp::resolver::query query(ep);
		endpoint_iterator = resolver->resolve(endpoint);
		socket=new tcp::socket(*io_service);
		boost::asio::connect(*socket, endpoint_iterator);
	}
	catch (exception & e)
	{

	}
}

